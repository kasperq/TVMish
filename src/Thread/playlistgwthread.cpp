#include "playlistgwthread.h"

PlaylistGwThread::PlaylistGwThread(DataBase &db) : m_db(std::make_shared< DataBase > (db))
{
    qDebug() << "PlaylistGwThread constructor";
    connect(&m_plGw, SIGNAL(deleted(const int&, const int&)), this, SIGNAL(deleted(const int&, const int&)));
    connect(&m_plGw, SIGNAL(getNewIdPlaylistAndNum(const int &, const int &, const uint &)),
            this, SIGNAL(getNewIdPlaylistAndNum(const int &, const int &, const uint &)));
//    connect(this, SIGNAL(finished()), &m_plGw, SLOT(deleteLater()));
    //    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

void PlaylistGwThread::insertRecord(const int &index, const QString &naim, const bool &isCurrent)
{
    m_plGw.insert(index, naim, isCurrent);
}

void PlaylistGwThread::run()
{
    qDebug() << "GWThread is running";
}

void PlaylistGwThread::deleteRecord(const int &idPlaylist, const int &index)
{
    m_plGw.deleteRecord(idPlaylist, index);
}

