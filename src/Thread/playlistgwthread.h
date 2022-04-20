#ifndef PLAYLISTGWTHREAD_H
#define PLAYLISTGWTHREAD_H

#include <QThread>
#include <QObject>

#include "DB/database.h"
#include "Gateways/playlistgw.h"

class PlaylistGwThread : public QThread
{
    Q_OBJECT
public:
    PlaylistGwThread(DataBase &db);


signals:
    void getNewIdPlaylistAndNum(const int &index, const int &idPlaylist, const uint &num);
    void deleted(const int &idPlaylist, const int &index);


public slots:
    void insertRecord(const int &index, const QString &naim, const bool &isCurrent);
    void deleteRecord(const int &idPlaylist, const int &index);

private:
    std::shared_ptr< DataBase > m_db;
    PlaylistGW m_plGw {this/*, m_db->db()*/};

    void run() override;
};

#endif // PLAYLISTGWTHREAD_H
