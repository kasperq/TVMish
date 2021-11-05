#include "playlistgw.h"

PlaylistGW::PlaylistGW(QSqlDatabase db) : m_db(db)
{
    select();
}

PlaylistGW::~PlaylistGW()
{

}

int *PlaylistGW::rows()
{
    return &m_rows;
}

void PlaylistGW::select()
{
    q_select = new QSqlQuery(m_db);
    q_select->prepare("select PLAYLIST.ID_PLAYLIST, PLAYLIST.NAIM, PLAYLIST.IS_CURRENT, coalesce(PLAYLIST.NUM,0) NUM "
                      "from PLAYLIST "
                      "order by PLAYLIST.NUM ");
    q_select->exec();
    calcRowCount();
}

void PlaylistGW::insert(Playlist *newItem)
{
    qDebug() << "insert";
    q_insert = new QSqlQuery(m_db);
    q_insert->prepare("insert into PLAYLIST "
                      "(ID_PLAYLIST, NAIM, IS_CURRENT) "
                      "values (:id_playlist, :naim, :is_current) ");
    q_insert->bindValue(":naim", newItem->naim());
    q_insert->bindValue(":is_current", newItem->isCurrent());

    int newId = getMaxId() + 1;
    qDebug() << "newId: " << newId;
    q_insert->bindValue(":id_playlist", newId);
    q_insert->exec();

    newItem->setIdPlaylist(&newId);
    uint num = getCurNum(&newId);
    qDebug() << "maxNum: " << num;
    newItem->setNum(&num);

    qDebug() << "inserted row";
}

void PlaylistGW::modify(int *idPlaylist, QString *naim, bool *isCurrent, uint *num)
{
    q_modify = new QSqlQuery(m_db);
    q_modify->prepare("update PLAYLIST "
                      "set ID_PLAYLIST = :id_playlist, NAIM = :naim, IS_CURRENT = :is_current, NUM = :num "
                      "where id_playlist = :id_playlist ");
    q_modify->bindValue(":id_playlist", *idPlaylist);
    q_modify->bindValue(":naim", *naim);
    q_modify->bindValue(":is_current", *isCurrent);
    q_modify->bindValue(":num", *num);
    q_modify->exec();

    qDebug() << "modified";
}

void PlaylistGW::deleteRecord(int *idPlaylist)
{
    q_delete = new QSqlQuery(m_db);
    q_delete->prepare("delete from PLAYLIST where id_playlist = :id_playlist ");
    q_delete->bindValue(":id_playlist", *idPlaylist);
    q_delete->exec();

    qDebug() << "deleted id_playlist=" << *idPlaylist;
}

QSqlQuery *PlaylistGW::data() const
{
    return q_select;
}

void PlaylistGW::calcRowCount()
{
    m_rows = 0;
    if (q_select->first()) {
        do {
            ++m_rows;
        }
        while (q_select->next());
    }

}

int PlaylistGW::getMaxId()
{
    q_temp = new QSqlQuery(m_db);
    q_temp->exec("select max(playlist.id_playlist) max_id from playlist");
    q_temp->first();
    return q_temp->value(0).toInt();
}

int PlaylistGW::getCurNum(int *id)
{
    q_temp = new QSqlQuery(m_db);
    q_temp->prepare("select playlist.num from playlist where playlist.id_playlist = :id_playlist ");
    q_temp->bindValue(":id_playlist", *id);
    q_temp->exec();
    q_temp->first();
    return q_temp->value(0).toInt();
}
