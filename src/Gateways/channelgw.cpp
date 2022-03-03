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
    qDebug() << "channelGW: select: " << idFile << " idPlaylit: " << idPlaylist;
    DBst::getInstance().startTransDB();
    q_select = QSqlQuery(DBst::getInstance().db_def());
    QString queryStr;
    queryStr = "select channels.ARCH_DAYS, channels.ID_CATEGORY, channels.ID_CHANNEL, channels.ID_FILE, "
               "channels.ID_LOGO, channels.ID_PLAYLIST, channels.NAIM, channels.NUM_FILE, channels.NUM_PLAYLIST, "
               "channels.url "
               "from channels ";
    if (idFile != 0)
        queryStr += "where channels.ID_FILE = :id_file ";
    q_select.prepare(queryStr);
    if (idFile != 0)
        q_select.bindValue(":id_file", idFile);
    if (q_select.exec()) {
        //    QFuture<QSqlQuery> future = DBst::getInstance().execAndGetQuery(q_select);
        //    qDebug() << "";
        //    q_select = future.result();
        q_select.first();
        calcRowCount();
        qDebug() << "channelGW: selected";
    } else {
        qDebug() << "channelGW::select error: " << q_select.lastError();
    }
    emit selected(idFile);
}

void ChannelGW::insert(const int &index, const QString &naim, const QString &url, const int &idFile, const int &idPlaylist,
                       const int &idCategory, const int &numFile, const int &numPlaylist, const int &archDays,
                       const int &idLogo, const bool &generateNums)
{
    DBst::getInstance().startTransDB();
    qDebug() << "channelGW: insert: " << naim;
    q_insert = QSqlQuery(DBst::getInstance().db_def());
    q_insert.clear();
    q_insert.prepare("insert into CHANNELS "
                     "(ID_CHANNEL, NAIM, URL, ID_FILE, ID_PLAYLIST, ID_CATEGORY, NUM_FILE, NUM_PLAYLIST, ARCH_DAYS, ID_LOGO) "
                     "values (:id_channel, :naim, :url, :id_file, :id_playlist, :id_category, :num_file, :num_playlist, :arch_days, :id_logo) ");
    q_insert.bindValue(":naim", naim);
    q_insert.bindValue(":url", url);
    q_insert.bindValue(":id_file", idFile);
    q_insert.bindValue(":id_playlist", idPlaylist);
    q_insert.bindValue(":id_category", idCategory);    
    q_insert.bindValue(":arch_days", archDays);
    q_insert.bindValue(":id_logo", idLogo);


    int maxNumFile {numFile};
    int maxNumPlaylist {numPlaylist};
    if (generateNums) {
        maxNumFile = getNumFile(idPlaylist, idFile) + 1;
        maxNumPlaylist = getNumPlaylist(idPlaylist) + 1;
    }
    q_insert.bindValue(":num_file", maxNumFile);
    q_insert.bindValue(":num_playlist", maxNumPlaylist);

    int newId = getMaxId() + 1;    
    q_insert.bindValue(":id_channel", newId);

    qDebug() << "channelGW: will insert";
    DBst::getInstance().execAndCheck(q_insert).then([this, newId, index, maxNumFile, maxNumPlaylist, idFile, idPlaylist, naim](bool result) {
        qDebug() << "channelGW: inserted: " << result << " naim: " << naim;
        if (result) {
//            save();
            q_insert.finish();
            qDebug() << "channelGW: inserted: commited";
            emit inserted(index, newId, maxNumFile, maxNumPlaylist, idFile, idPlaylist);
        }
    });
}

void ChannelGW::modify(const int &index, const int &idChannel, const QString &naim, const QString &url, const int &idFile,
                       const int &idPlaylist, const int &idCategory, const int &numFile, const int &numPlaylist,
                       const int &archDays, const int &idLogo)
{
    DBst::getInstance().startTransDB();
    q_modify = QSqlQuery(DBst::getInstance().db_def());
    q_modify.prepare("update CHANNELS "
                     "set "
                     "ID_CHANNEL = :id_channel, "
                     "NAIM = :naim, "
                     "URL = :url, "
                     "ID_FILE = :id_file, "
                     "ID_PLAYLIST = :id_playlist, "
                     "ID_CATEGORY = :id_category, "
                     "NUM_FILE = :num_file, "
                     "NUM_PLAYLIST = :num_playlist, "
                     "ARCH_DAYS = :arch_days, "
                     "ID_LOGO = :id_logo "
                     "where id_channel = :id_channel ");
    q_modify.bindValue(":id_channel", idChannel);
    q_modify.bindValue(":naim", naim);
    q_modify.bindValue(":url", url);
    q_modify.bindValue(":id_file", idFile);
    q_modify.bindValue(":id_playlist", idPlaylist);
    q_modify.bindValue(":id_category", idCategory);
    q_modify.bindValue(":num_file", numFile);
    q_modify.bindValue(":num_playlist", numPlaylist);
    q_modify.bindValue(":arch_days", archDays);
    q_modify.bindValue(":id_logo", idLogo);

    DBst::getInstance().execAndCheck(q_modify).then([this, index](bool result) {
        if (result) {
            save();
            q_modify.finish();
            emit modified(index);
        }
    });
}

void ChannelGW::deleteRecord(const int &index, const int &idChannel)
{
    DBst::getInstance().startTransDB();
    q_delete = QSqlQuery(DBst::getInstance().db_def());
    q_delete.prepare("delete from CHANNELS where id_channel = :id_channel ");
    q_delete.bindValue(":id_channel", idChannel);

    DBst::getInstance().execAndCheck(q_delete).then([this, idChannel, index](bool result) {
        if (result) {
            save();
            q_delete.finish();
            emit deleted(index, idChannel);
        }
    });
}

void ChannelGW::deleteAllInFile(const int &index, const int &idFile)
{
    DBst::getInstance().startTransDB();
    q_delete = QSqlQuery(DBst::getInstance().db_def());
    q_delete.prepare("delete from CHANNELS where id_file = :id_file ");
    q_delete.bindValue(":id_file", idFile);

    DBst::getInstance().execAndCheck(q_delete).then([this, idFile, index](bool result) {
        if (result) {
            save();
            q_delete.finish();
            emit deletedAllInFile(index, idFile);
        }
    });
}

void ChannelGW::deleteAllInPlaylist(const int &index, const int &idPlaylist)
{
    DBst::getInstance().startTransDB();
    q_delete = QSqlQuery(DBst::getInstance().db_def());
    q_delete.prepare("delete from CHANNELS where id_playlist = :id_playlist ");
    q_delete.bindValue(":id_file", idPlaylist);

    DBst::getInstance().execAndCheck(q_delete).then([this, idPlaylist, index](bool result) {
        if (result) {
            save();
            q_delete.finish();
            emit deletedAllInPlaylist(index, idPlaylist);
        }
    });
}

void ChannelGW::getNums(const int &index, const int &idPlaylist, const int &idFile)
{
    int maxNumFile = getNumFile(idPlaylist, idFile);
    int maxNumPlaylist = getNumPlaylist(idPlaylist);

    emit numsRecieved(index, idPlaylist, idFile, maxNumPlaylist, maxNumFile);
}

int ChannelGW::getNumFile(const int &idPlaylist, const int &idFile)
{    
    DBst::getInstance().startTransDB();
//    q_temp.clear();
    q_temp = QSqlQuery(DBst::getInstance().db_def());
    q_temp.prepare("select coalesce(max(channels.NUM_FILE), 0) max_num "
                   "from channels "
                   "where channels.ID_FILE = :id_file "
                   "and channels.ID_PLAYLIST = :id_playlist ");
    q_temp.bindValue(":id_file", idFile);
    q_temp.bindValue(":id_playlist", idPlaylist);
    qDebug() << "getNumFile: willExec";
    try {
//        QFuture<QSqlQuery> future = DBst::getInstance().execAndGetQuery(q_temp);
//        try {
//        q_temp = future.result();
//        } catch (...) {
//            qDebug() << "";
//        }
        if (q_temp.exec()) {
            qDebug() << "getNumFile: execed";
            q_temp.first();
            int maxNumFile {};
            if (q_temp.isValid())
                maxNumFile = q_temp.value(0).toInt();
            q_temp.finish();
            return maxNumFile;
        } else {
            qDebug() << "channelGW::getNumFile error: " << q_temp.lastError();
        }
    } catch (...) {
        qDebug() << "error: " << q_temp.lastError();;
    }
    return 0;
}

int ChannelGW::getNumPlaylist(const int &idPlaylist)
{    
    DBst::getInstance().startTransDB();
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

void ChannelGW::addForInsert(const int &index, const QString &naim, const QString &url, const int &idFile,
                             const int &idPlaylist, const int &idCategory, const int &numFile, const int &numPlaylist,
                             const int &archDays, const int &idLogo, const bool &generateNums)
{
    qDebug() << "channelGW: insert: " << naim;
    if (!m_isPreparedForInsert) {
        prepareInsert();
        m_newId = getMaxId() + 1;
        if (generateNums) {
            m_maxNumFile = getNumFile(idPlaylist, idFile) + 1;
            m_maxNumPlaylist = getNumPlaylist(idPlaylist) + 1;
        } else {
            m_maxNumFile = numFile;
            m_maxNumPlaylist = numPlaylist;
        }
    } else {
        m_newId++;
        if (generateNums) {
            m_maxNumFile++;
            m_maxNumPlaylist++;
        } else {
            m_maxNumFile = numFile;
            m_maxNumPlaylist = numPlaylist;
        }
    }
    QString s_idLogo;
    if (idLogo == 0) {
        s_idLogo = "NULL";
    } else {
        s_idLogo = QString::number(idLogo);
    }

    qDebug() << "channelGW: inserted: " << naim;
//    m_insertText += QString("insert into CHANNELS "
//                    "(ID_CHANNEL, NAIM, URL, ID_FILE, ID_PLAYLIST, ID_CATEGORY, NUM_FILE, NUM_PLAYLIST, ARCH_DAYS, ID_LOGO) "
//                    "values (%1, ''%2'', ''%3'', %4, %5, %6, %7, %8, %9, %10); ")
//            .arg(newId)
//            .arg(naim)
//            .arg(url)
//            .arg(idFile)
//            .arg(idPlaylist)
//            .arg(idCategory)
//            .arg(maxNumFile)
//            .arg(maxNumPlaylist)
//            .arg(archDays)
//            .arg(idLogo);
    m_insertText += QString("insert into CHANNELS "
                    "(ID_CHANNEL, ID_FILE, ID_PLAYLIST, ID_CATEGORY, NUM_FILE, NUM_PLAYLIST, ARCH_DAYS, ID_LOGO) "
                    "values (%1, %4, %5, %6, %7, %8, %9, %10); ")
            .arg(m_newId)
            .arg(idFile)
            .arg(idPlaylist)
            .arg(idCategory)
            .arg(m_maxNumFile)
            .arg(m_maxNumPlaylist)
            .arg(archDays)
            .arg(s_idLogo);

    emit inserted(index, m_newId, m_maxNumFile, m_maxNumPlaylist, idFile, idPlaylist);
}

void ChannelGW::saveInsert()
{
    if (!m_insertText.isEmpty()) {
        m_insertText += "COMMIT; ";
        DBst::getInstance().startTransDB();
        q_insert = QSqlQuery(DBst::getInstance().db_def());
        m_insertText = "insert into CHANNELS (ID_CHANNEL, ID_FILE, ID_PLAYLIST, ID_CATEGORY, NUM_FILE, NUM_PLAYLIST, ARCH_DAYS, ID_LOGO) values (331, 47, 2, 2, 331, 331, 0, NULL); insert into CHANNELS (ID_CHANNEL, ID_FILE, ID_PLAYLIST, ID_CATEGORY, NUM_FILE, NUM_PLAYLIST, ARCH_DAYS, ID_LOGO) values (332, 47, 2, 2, 332, 332, 0, NULL)";
        q_insert.prepare(m_insertText);
        qDebug() << "will exec: " << m_insertText;
//        m_insertText = "insert into CHANNELS (ID_CHANNEL, ID_FILE, ID_PLAYLIST, ID_CATEGORY, NUM_FILE, NUM_PLAYLIST, ARCH_DAYS, ID_LOGO) values (331, 47, 2, 2, 331, 331, 0, NULL); insert into CHANNELS (ID_CHANNEL, ID_FILE, ID_PLAYLIST, ID_CATEGORY, NUM_FILE, NUM_PLAYLIST, ARCH_DAYS, ID_LOGO) values (332, 47, 2, 2, 332, 332, 0, NULL)";
        if (q_insert.exec()) {
//        DBst::getInstance().execAndCheck(q_insert).then([this](bool result) {
//            if (result) {
                save();
                m_isPreparedForInsert = false;
                q_insert.finish();
                qDebug() << "channelGW: inserted: commited";
            } else {
            qDebug() << "error: " << q_insert.lastError();
        }
//        });
    }
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
    DBst::getInstance().startTransDB();
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
