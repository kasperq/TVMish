#include "playlistgw.h"

#include "DB/dbst.h"

PlaylistGW::PlaylistGW(QObject *parent/*, QSqlDatabase db*/) : QObject(parent)/*, m_db(db)*/
{
    qDebug() << "PlaylistGW constructor";
//    select();
}

PlaylistGW::~PlaylistGW()
{

}

int PlaylistGW::rows() const
{
    return m_rows;
}

void PlaylistGW::select()
{
//    q_select = new QSqlQuery(m_db);
    DBst::getInstance().startTransDB();
    q_select = QSqlQuery(/*m_db*/DBst::getInstance().db_def());
    q_select.prepare("select PLAYLIST.ID_PLAYLIST, PLAYLIST.NAIM, PLAYLIST.IS_CURRENT, coalesce(PLAYLIST.NUM,0) NUM "
                      "from PLAYLIST "
                      "order by PLAYLIST.NUM ");
//    q_select.exec();
    QFuture<QSqlQuery> future = DBst::getInstance().execAndGetQuery(q_select);
    qDebug() << "";
    q_select = future.result();
    calcRowCount();

    emit selected();
}

void PlaylistGW::insert(Playlist *newItem)
{    
    DBst::getInstance().startTransDB();
    q_insert = QSqlQuery(/*m_db*/DBst::getInstance().db_def());
    q_insert.prepare("insert into PLAYLIST "
                      "(ID_PLAYLIST, NAIM, IS_CURRENT) "
                      "values (:id_playlist, :naim, :is_current) ");
    q_insert.bindValue(":naim", newItem->naim());
    q_insert.bindValue(":is_current", newItem->isCurrent());

    int newId = getMaxId() + 1;
    qDebug() << "newId: " << newId;
    q_insert.bindValue(":id_playlist", newId);

    DBst::getInstance().execAndCheck(q_insert).then([this, newItem, newId](bool result) {
        if (result) {
            DBst::getInstance().commitDbDef();
            q_insert.finish();
            newItem->setIdPlaylist(newId);
            uint num = getCurNum(newId);
            qDebug() << "maxNum: " << num;
            newItem->setNum(num);
        }
    });

//    q_insert.exec();
//    newItem->setIdPlaylist(newId);
//    uint num = getCurNum(newId);
//    qDebug() << "maxNum: " << num;
//    newItem->setNum(num);
}

void PlaylistGW::insert(const int &index, const QString &naim, const bool &isCurrent)
{
    DBst::getInstance().startTransDB();
    q_insert = QSqlQuery(/*m_db*/DBst::getInstance().db_def());
    q_insert.prepare("insert into PLAYLIST "
                      "(ID_PLAYLIST, NAIM, IS_CURRENT) "
                      "values (:id_playlist, :naim, :is_current) ");
    q_insert.bindValue(":naim", naim);
    q_insert.bindValue(":is_current", isCurrent);

    int newId = getMaxId() + 1;

    q_insert.bindValue(":id_playlist", newId);

    DBst::getInstance().execAndCheck(q_insert).then([this, newId, index](bool result) {
        if (result) {
            uint num = getCurNum(newId);
            DBst::getInstance().commitDbDef();
            q_insert.finish();
            emit getNewIdPlaylistAndNum(index, newId, num);
        }
    });
//    q_insert.exec();
//    uint num = getCurNum(newId);
//    q_insert.finish();

//    emit getNewIdPlaylistAndNum(index, newId, num);
}

void PlaylistGW::modify(const int &idPlaylist, const QString &naim, const bool &isCurrent, const uint &num)
{    
    DBst::getInstance().startTransDB();
    q_modify = QSqlQuery(/*m_db*/DBst::getInstance().db_def());
    q_modify.prepare("update PLAYLIST "
                      "set ID_PLAYLIST = :id_playlist, NAIM = :naim, IS_CURRENT = :is_current, NUM = :num "
                      "where id_playlist = :id_playlist ");
    q_modify.bindValue(":id_playlist", idPlaylist);
    q_modify.bindValue(":naim", naim);
    q_modify.bindValue(":is_current", isCurrent);
    q_modify.bindValue(":num", num);

    DBst::getInstance().execAndCheck(q_modify).then([this](bool result) {
        if (result) {
            DBst::getInstance().commitDbDef();
            q_modify.finish();
        }
    });
//    q_modify.exec();
//    q_modify.finish();
}

void PlaylistGW::deleteRecord(const int &idPlaylist, const int &index)
{        
    qDebug() << "PlGW: deleteRecord: idPlaylist: " << idPlaylist << " index: " << index;
    DBst::getInstance().startTransDB();
    q_delete = QSqlQuery(/*m_db*/DBst::getInstance().db_def());
    q_delete.prepare("delete from PLAYLIST where id_playlist = :id_playlist ");
    q_delete.bindValue(":id_playlist", idPlaylist);

    DBst::getInstance().execAndCheck(q_delete).then([this, idPlaylist, index](bool result) {
        if (result) {
            DBst::getInstance().commitDbDef();
            q_delete.finish();
            emit deleted(idPlaylist, index);
        }
    });
//    q_delete.exec();
//    q_delete.finish();
//    emit deleted(idPlaylist, index);
}

QSqlQuery *PlaylistGW::data() const
{
    return const_cast<QSqlQuery*>(&q_select);
}

void PlaylistGW::calcRowCount()
{
    m_rows = 0;    
    if (q_select.first()) {
        do {
            ++m_rows;
        } while (q_select.next());
        q_select.first();
    }
}

int PlaylistGW::getMaxId()
{
    DBst::getInstance().startTransDB();
    q_temp = QSqlQuery(/*m_db*/DBst::getInstance().db_def());
    q_temp.prepare("select max(playlist.id_playlist) max_id from playlist");

    QFuture<QSqlQuery> future = DBst::getInstance().execAndGetQuery(q_temp);
    qDebug() << "";
    q_temp = future.result();

    q_temp.first();
    int maxId {q_temp.value(0).toInt()};
    q_temp.finish();
    return maxId;
}

int PlaylistGW::getCurNum(const int &id)
{
    DBst::getInstance().startTransDB();
    q_temp = QSqlQuery(/*m_db*/DBst::getInstance().db_def());
    q_temp.prepare("select playlist.num from playlist where playlist.id_playlist = :id_playlist ");
    q_temp.bindValue(":id_playlist", id);

    QFuture<QSqlQuery> future = DBst::getInstance().execAndGetQuery(q_temp);
    qDebug() << "";
    q_temp = future.result();

//    q_temp.exec();
    q_temp.first();
    int newId {q_temp.value(0).toInt()};
    q_temp.finish();
    return newId;
}
