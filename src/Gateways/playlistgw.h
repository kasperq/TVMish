#ifndef PLAYLISTGW_H
#define PLAYLISTGW_H

//#include <QSqlDatabase>
#include <QSqlQuery>

#include "./Classes/playlist.h"

class PlaylistGW
{
public:
    PlaylistGW(QSqlDatabase db);
    virtual ~PlaylistGW();

    int* rows();

    void select();
    void insert(Playlist *newItem);
    void modify(int *idPlaylist, QString *naim, bool *isCurrent, uint *num);
    void deleteRecord(int *idPlaylist);

    QSqlQuery *data() const;

private:
    QSqlDatabase m_db;

    QSqlQuery *q_select;
    QSqlQuery *q_modify;
    QSqlQuery *q_insert;
    QSqlQuery *q_delete;
    QSqlQuery *q_temp;

    int m_rows;

    void calcRowCount();
    int getMaxId();
    int getCurNum(int *id);

};

#endif // PLAYLISTGW_H
