#include "channelgw.h"

#include "./DB/dbst.h"

#include <QSqlError>

ChannelGW::ChannelGW(QObject *parent)
    : QObject{parent}
{

}

ChannelGW::~ChannelGW()
{

}

QSqlQuery *ChannelGW::data() const
{
    return const_cast<QSqlQuery*>(&q_select);
}

int ChannelGW::rows() const
{
    return m_rows;
}

void ChannelGW::select(const int &idPlaylist, const int &idFile)
{    
    DBst::getInstance().startTransDBdef();
    q_select = QSqlQuery(DBst::getInstance().db_def());
    QString queryStr;    
    q_select.clear();
    queryStr = "select channels.ARCH_DAYS, channels.ID_CATEGORY, channels.ID_CHANNEL, channels.ID_FILE, "
               "channels.ID_LOGO, channels.ID_PLAYLIST, channels.NAIM, channels.NUM_PLAYLIST, "
               "channels.url, category.NAME category_naim, "
               "coalesce(logos.LOGO_PATH, (select logos.LOGO_PATH from logos where logos.ID_LOGO = 1)) LOGO_PATH,"
               "channels.is_favorite, channels.num_favorite, channels.is_available, channels.naim_lower "
               "from channels "
               "left join category on category.ID_CATEGORY = channels.ID_CATEGORY "
               "left join logos on logos.ID_LOGO = channels.ID_LOGO "
               "where channels.ID_PLAYLIST = :id_playlist ";

    if (idFile != 0)
        queryStr += "and channels.ID_FILE = :id_file ";
    if (!m_filterNaim.isEmpty())
        queryStr += "and trim(channels.NAIM_LOWER) like trim(\"%" + m_filterNaim +"%\") ";
    if (m_idCategory != 0)
        queryStr += "and channels.ID_CATEGORY = " + QString::number(m_idCategory) + " ";
    queryStr += "order by iif(coalesce(channels.is_favorite, false) = true, channels.num_favorite, channels.num_playlist) ";

    q_select.prepare(queryStr);
    q_select.bindValue(":id_playlist", idPlaylist);
    if (idFile != 0)
        q_select.bindValue(":id_file", idFile);
    if (q_select.exec()) {
        //    QFuture<QSqlQuery> future = DBst::getInstance().execAndGetQuery(q_select);
        //    qDebug() << "";
        //    q_select = future.result();
        q_select.first();
        calcRowCount();
    } else {
        qDebug() << "channelGW::select error: " << q_select.lastError();
    }
    emit selected(idPlaylist, idFile);
}

void ChannelGW::selectWithFilter(const int &idPlaylist, const int &idFile, const QString &filterNaim, const int &idCategory)
{
//    qDebug() << "ChannelGW::selectWithFilter: idPl: " << idPlaylist << " idF: " << idFile << " fil: " << filterNaim;
    m_filterNaim = filterNaim;
    m_idCategory = idCategory;
    select(idPlaylist, idFile);
}

void ChannelGW::insert(const int &index, const QString &naim, const QString &url, const int &idFile, const int &idPlaylist,
                       const int &idCategory, const int &numPlaylist, const int &archDays, const int &idLogo,
                       const bool &generateNums, const bool &isFavorite, const int &numFav, const bool &isAvailable)
{
    if (!m_isPreparedForInsert) {
        prepareInsert();
        m_newId = getMaxId() + 1;
        if (generateNums) {            
            m_maxNumPlaylist = getNumPlaylist(idPlaylist) + 1;
        } else {
            m_maxNumPlaylist = numPlaylist;
        }
    } else {
        m_newId++;
        if (generateNums) {
            m_maxNumPlaylist++;
        } else {            
            m_maxNumPlaylist = numPlaylist;
        }
    }
    DBst::getInstance().startTransDB();
    qDebug() << "channelGW: insert: " << naim;
    q_insert = QSqlQuery(DBst::getInstance().db_def());
    q_insert.clear();
    q_insert.prepare("insert into CHANNELS "
                     "(ID_CHANNEL, NAIM, URL, ID_FILE, ID_PLAYLIST, ID_CATEGORY, NUM_PLAYLIST, ARCH_DAYS, "
                     "ID_LOGO, IS_FAVORITE, NUM_FAVORITE, IS_AVAILABLE, NAIM_LOWER) "
                     "values (:id_channel, :naim, :url, :id_file, :id_playlist, :id_category, :num_playlist, "
                     ":arch_days, :id_logo, :is_favorite, :num_favorite, :is_available, :naim_lower) ");
    q_insert.bindValue(":naim", naim);
    q_insert.bindValue(":url", url);
    q_insert.bindValue(":id_file", idFile);
    q_insert.bindValue(":id_playlist", idPlaylist);
    q_insert.bindValue(":id_category", idCategory);    
    q_insert.bindValue(":arch_days", archDays);
    q_insert.bindValue(":id_logo", idLogo);
    q_insert.bindValue(":is_favorite", isFavorite);
    q_insert.bindValue(":num_favorite", numFav);
    q_insert.bindValue(":is_available", isAvailable);
    q_insert.bindValue(":naim_lower", naim.toLower());

    q_insert.bindValue(":num_playlist", m_maxNumPlaylist);
    q_insert.bindValue(":id_channel", m_newId);


    DBst::getInstance().execAndCheck(q_insert).then([this, naim, index, idFile, idPlaylist](bool result) {
        qDebug() << "channelGW: inserted: " << result << " naim: " << naim;
        if (result) {
            q_insert.finish();
            qDebug() << "channelGW: inserted: finished";
            emit inserted(index, m_newId, m_maxNumPlaylist, idFile, idPlaylist);
        }
    });

//    try {
//    if (q_insert.exec()) {
//            q_insert.finish();
//            qDebug() << "channelGW: inserted: commited";
//            emit inserted(index, m_newId, m_maxNumPlaylist, idFile, idPlaylist);
//        } else {
//        qDebug() << "channelGW error: " << q_insert.lastError();
//    }
//    } catch (...) {
//    }

}

void ChannelGW::modify(const int &index, const int &idChannel, const QString &naim, const QString &url, const int &idFile,
                       const int &idPlaylist, const int &idCategory, const int &numPlaylist, const int &archDays,
                       const int &idLogo, const bool &isFavorite, const int &numFav, const bool &isAvailable)
{
    DBst::getInstance().startTransDBdef();
    q_modify = QSqlQuery(DBst::getInstance().db_def());
    q_modify.prepare("update CHANNELS "
                     "set "
                     "ID_CHANNEL = :id_channel, "
                     "NAIM = :naim, "
                     "URL = :url, "
                     "ID_FILE = :id_file, "
                     "ID_PLAYLIST = :id_playlist, "
                     "ID_CATEGORY = :id_category, "                    
                     "NUM_PLAYLIST = :num_playlist, "
                     "ARCH_DAYS = :arch_days, "
                     "ID_LOGO = :id_logo,"
                     "IS_FAVORITE = :is_favorite,"
                     "NUM_FAVORITE = :num_favorite,"
                     "IS_AVAILABLE = :is_available,"
                     "NAIM_LOWER = :naim_lower "
                     "where id_channel = :id_channel ");
    q_modify.bindValue(":id_channel", idChannel);
    q_modify.bindValue(":naim", naim);
    q_modify.bindValue(":url", url);
    q_modify.bindValue(":id_file", idFile);
    q_modify.bindValue(":id_playlist", idPlaylist);
    q_modify.bindValue(":id_category", idCategory);
    q_modify.bindValue(":num_playlist", numPlaylist);
    q_modify.bindValue(":arch_days", archDays);
    q_modify.bindValue(":id_logo", idLogo);
    q_modify.bindValue(":is_favorite", isFavorite);
    q_modify.bindValue(":num_favorite", numFav);
    q_modify.bindValue(":is_available", isAvailable);
    q_modify.bindValue(":naim_lower", naim.toLower());

//    DBst::getInstance().execAndCheck(q_modify).then([this, index](bool result) {
//        if (result) {
    if (q_modify.exec()) {
            save();
            q_modify.finish();
            emit modified(index);
        }
    //    });
}

void ChannelGW::setFavorite(const int &index, const bool &isFavorite, const int &idChannel, const int &idPlaylist)
{
    DBst::getInstance().startTransDBdef();
    q_modify = QSqlQuery(DBst::getInstance().db_def());
    q_modify.prepare("update CHANNELS "
                     "set "
                     "IS_FAVORITE = :is_favorite,"
                     "NUM_FAVORITE = :num_favorite "
                     "where id_channel = :id_channel ");
    q_modify.bindValue(":id_channel", idChannel);
    q_modify.bindValue(":is_favorite", isFavorite);
    int numFav {};
    if (isFavorite)
        numFav = getMaxNumFavorite(idPlaylist) + 1;
    q_modify.bindValue(":num_favorite", numFav);

    if (q_modify.exec()) {
        save();
        q_modify.finish();
        emit sendFavoriteResult(index, idChannel, numFav);
    }
}

void ChannelGW::deleteRecord(const int &index, const int &idChannel)
{
    DBst::getInstance().startTransDBdef();
    q_delete = QSqlQuery(DBst::getInstance().db_def());
    q_delete.prepare("delete from CHANNELS where id_channel = :id_channel ");
    q_delete.bindValue(":id_channel", idChannel);

//    DBst::getInstance().execAndCheck(q_delete).then([this, idChannel, index](bool result) {
//        if (result) {
    if (q_delete.exec()) {
            save();
            q_delete.finish();
            emit deleted(index, idChannel);
        }
//    });
}

void ChannelGW::deleteAllInFile(const int &index, const int &idFile)
{
    DBst::getInstance().startTransDBdef();
    q_delete = QSqlQuery(DBst::getInstance().db_def());
    q_delete.prepare("delete from CHANNELS where id_file = :id_file ");
    q_delete.bindValue(":id_file", idFile);

//    DBst::getInstance().execAndCheck(q_delete).then([this, idFile, index](bool result) {
//        if (result) {
    if (q_delete.exec()) {
            save();
            q_delete.finish();
            emit deletedAllInFile(idFile);
        }
//    });
}

void ChannelGW::deleteAllInPlaylist(const int &index, const int &idPlaylist)
{
    DBst::getInstance().startTransDBdef();
    q_delete = QSqlQuery(DBst::getInstance().db_def());
    q_delete.prepare("delete from CHANNELS where id_playlist = :id_playlist ");
    q_delete.bindValue(":id_file", idPlaylist);

//    DBst::getInstance().execAndCheck(q_delete).then([this, idPlaylist, index](bool result) {
//        if (result) {
    if (q_delete.exec()) {
            save();
            q_delete.finish();
            emit deletedAllInPlaylist(index, idPlaylist);
        }
//    });
}

void ChannelGW::getNums(const int &index, const int &idPlaylist, const int &idFile)
{
    int maxNumPlaylist = getNumPlaylist(idPlaylist);

    emit numsRecieved(index, idPlaylist, idFile, maxNumPlaylist);
}

int ChannelGW::getNumPlaylist(const int &idPlaylist)
{    
    DBst::getInstance().startTransDBdef();
    q_temp = QSqlQuery(DBst::getInstance().db_def());
    q_temp.clear();
    q_temp.prepare("select coalesce(max(channels.NUM_PLAYLIST), 0) max_num "
                   "from channels "
                   "where channels.ID_PLAYLIST = :id_playlist ");
    q_temp.bindValue(":id_playlist", idPlaylist);
    qDebug() << "getNumPlaylist: willExec";
    try {
        if (q_temp.exec()) {
            qDebug() << "getNumPlaylist: execed";
            q_temp.first();
            int maxNumPlaylist {};
            if (q_temp.isValid())
                maxNumPlaylist = q_temp.value(0).toInt();
            q_temp.finish();
            return maxNumPlaylist;
        } else {
            qDebug() << "channelGW::getNumPlaylist error: " << q_temp.lastError();
        }
    } catch (...) {
        qDebug() << "error: " << q_temp.lastError();;
    }
    return 0;
}

void ChannelGW::save()
{
    qDebug() << "ChannelGW: save() : will be commiting ";
    bool res = DBst::getInstance().commitDbDef();
    qDebug() << "ChannelGW: save() : commited: " << res;
}

void ChannelGW::prepareInsert()
{
    m_insertText.clear();
    m_isPreparedForInsert = true;
}

void ChannelGW::calcRowCount()
{
    m_rows = 0;
    if (q_select.first()) {
        do {
            ++m_rows;
        }
        while (q_select.next());
        q_select.first();
    }
}

int ChannelGW::getMaxId()
{
    DBst::getInstance().startTransDBdef();
    q_temp = QSqlQuery(DBst::getInstance().db_def());    
    q_temp.prepare("select coalesce(max(channels.id_channel),0) max_id from channels ");
    try {
//        QFuture<QSqlQuery> future = DBst::getInstance().execAndGetQuery(q_temp);
//        try {
//        q_temp = future.result();
//        } catch (...) {
//            qDebug() << "";
//        }
        qDebug() << "getMaxId: will exec";
        if (q_temp.exec()) {
            qDebug() << "getMaxId: execed";
            q_temp.first();
            int maxId {};
            if (q_temp.isValid())
                maxId = q_temp.value(0).toInt();
            q_temp.finish();
            return maxId;
        } else {
            qDebug() << "channelGW::getMaxId error: " << q_temp.lastError();
        }
    } catch (...) {
        qDebug() << "error getMaxId: " << q_temp.lastError();
    }
    return 0;
}

int ChannelGW::getMaxNumFavorite(const int &idPlaylist)
{
    DBst::getInstance().startTransDBdef();
    q_temp = QSqlQuery(DBst::getInstance().db_def());
    q_temp.clear();
    q_temp.prepare("select coalesce(max(channels.num_favorite),0) max_num_favorite "
                   "from channels "
                   "where channels.id_playlist = :id_playlist ");
    q_temp.bindValue(":id_playlist", idPlaylist);
    try {
        if (q_temp.exec()) {
            q_temp.first();
            int maxNum {};
            if (q_temp.isValid())
                maxNum = q_temp.value(0).toInt();
            qDebug() << "maxNumFav: " << maxNum;
            q_temp.finish();
            return maxNum;
        } else {
            qDebug() << "channelGW::getMaxNumFavorite error: " << q_temp.lastError();
        }
    } catch (...) {
        qDebug() << "error getMaxId: " << q_temp.lastError();
    }
    return 0;
}
