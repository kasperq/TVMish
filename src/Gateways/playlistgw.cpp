#include "playlistgw.h"

PlaylistGW::PlaylistGW(QObject *parent, QSqlDatabase db) : QObject(parent), m_db(db)
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
    q_select = QSqlQuery(m_db);
    q_select.prepare("select PLAYLIST.ID_PLAYLIST, PLAYLIST.NAIM, PLAYLIST.IS_CURRENT, coalesce(PLAYLIST.NUM,0) NUM "
                      "from PLAYLIST "
                      "order by PLAYLIST.NUM ");
    q_select.exec();
    calcRowCount();

    emit selected();
}

void PlaylistGW::insert(Playlist *newItem)
{    
    q_insert = QSqlQuery(m_db);
    q_insert.prepare("insert into PLAYLIST "
                      "(ID_PLAYLIST, NAIM, IS_CURRENT) "
                      "values (:id_playlist, :naim, :is_current) ");
    q_insert.bindValue(":naim", newItem->naim());
    q_insert.bindValue(":is_current", newItem->isCurrent());

    int newId = getMaxId() + 1;
    qDebug() << "newId: " << newId;
    q_insert.bindValue(":id_playlist", newId);
    q_insert.exec();

    newItem->setIdPlaylist(newId);
    uint num = getCurNum(newId);
    qDebug() << "maxNum: " << num;
    newItem->setNum(num);
}

void PlaylistGW::insert(const int &index, const QString &naim, const bool &isCurrent)
{
    q_insert = QSqlQuery(m_db);
    q_insert.prepare("insert into PLAYLIST "
                      "(ID_PLAYLIST, NAIM, IS_CURRENT) "
                      "values (:id_playlist, :naim, :is_current) ");
    q_insert.bindValue(":naim", naim);
    q_insert.bindValue(":is_current", isCurrent);

    int newId = getMaxId() + 1;

    q_insert.bindValue(":id_playlist", newId);
    q_insert.exec();
    uint num = getCurNum(newId);
    q_insert.finish();

    emit getNewIdPlaylistAndNum(index, newId, num);
}

void PlaylistGW::modify(const int &idPlaylist, const QString &naim, const bool &isCurrent, const uint &num)
{    
    q_modify = QSqlQuery(m_db);
    q_modify.prepare("update PLAYLIST "
                      "set ID_PLAYLIST = :id_playlist, NAIM = :naim, IS_CURRENT = :is_current, NUM = :num "
                      "where id_playlist = :id_playlist ");
    q_modify.bindValue(":id_playlist", idPlaylist);
    q_modify.bindValue(":naim", naim);
    q_modify.bindValue(":is_current", isCurrent);
    q_modify.bindValue(":num", num);
    q_modify.exec();
    q_modify.finish();
}

void PlaylistGW::deleteRecord(const int &idPlaylist, const int &index)
{        
    q_delete = QSqlQuery(m_db);
    q_delete.prepare("delete from PLAYLIST where id_playlist = :id_playlist ");
    q_delete.bindValue(":id_playlist", idPlaylist);
    q_delete.exec();    

    q_delete.finish();
    emit deleted(idPlaylist, index);
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
        }
        while (q_select.next());
    }

}

int PlaylistGW::getMaxId()
{
    q_temp = QSqlQuery(m_db);
    q_temp.exec("select max(playlist.id_playlist) max_id from playlist");
    q_temp.first();
    int maxId {q_temp.value(0).toInt()};
    q_temp.finish();
    return maxId;
}

int PlaylistGW::getCurNum(const int &id)
{
    q_temp = QSqlQuery(m_db);
    q_temp.prepare("select playlist.num from playlist where playlist.id_playlist = :id_playlist ");
    q_temp.bindValue(":id_playlist", id);
    q_temp.exec();
    q_temp.first();
    int newId {q_temp.value(0).toInt()};
    q_temp.finish();
    return newId;
}
