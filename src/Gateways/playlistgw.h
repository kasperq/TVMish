#ifndef PLAYLISTGW_H
#define PLAYLISTGW_H

//#include <QSqlDatabase>
#include <QObject>
#include <QSqlQuery>
#include <QTime>
#include <QSqlError>

#include "./Classes/playlist.h"

class PlaylistGW : public QObject
{
    Q_OBJECT
public:
    PlaylistGW(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    virtual ~PlaylistGW();

    int rows() const;

    void select();
    void insert(Playlist *newItem);    

    QSqlQuery *data() const;    

signals:
    void selected();
    void getNewIdPlaylistAndNum(const int &index, const int &idPlaylist, const uint &num);
    void deleted(const int &idPlaylist, const int &index);

public slots:
    void insert(const int &index, const QString &naim, const bool &isCurrent);
    void modify(const int &idPlaylist, const QString &naim, const bool &isCurrent, const uint &num);
    void deleteRecord(const int &idPlaylist, const int &index);

private:
    QSqlDatabase m_db;

    QSqlQuery q_select;
    QSqlQuery q_modify;
    QSqlQuery q_insert;
    QSqlQuery q_delete;
    QSqlQuery q_temp;

    int m_rows;

    void calcRowCount();
    int getMaxId();
    int getCurNum(const int &id);

};

#endif // PLAYLISTGW_H
