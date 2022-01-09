#ifndef PLAYLISTCONTROLLER_H
#define PLAYLISTCONTROLLER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QSharedPointer>
#include <QThread>

#include "Gateways/playlistgw.h"
#include "Classes/playlists.h"
#include "Thread/playlistgwthread.h"

#include "Gateways/plfilegw.h"
#include "Classes/plfiles.h"

#include "Classes/settings.h"

#include "DB/database.h"

class PlaylistController : public QObject
{
    Q_OBJECT
public:
    PlaylistController(QQmlApplicationEngine &engine, DataBase &db);
//    PlaylistController();
//    PlaylistController& operator=(PlaylistController &orig);
    virtual ~PlaylistController();

    void openPlaylistManager();

signals:
    void filesAdded(const int &idPlaylist);

public slots:
    void addItemsFromDbToPlaylists();
    void addItemsFromDbToFiles();

private:
    QQmlApplicationEngine *m_engine;
    std::shared_ptr< DataBase > m_db;
    std::shared_ptr< Settings > m_sets;

    PlaylistGW m_plGw {this, m_db->db()};
    Playlists m_plLists;

    PlaylistGwThread m_plGwThread {*m_db};

    PlFiles m_files /*{*m_sets}*/;
    PlFileGW m_fileGW {this, m_db->db()};




    void initPlaylistConnections();
    void openPlaylists();

    void initFilesConnections();
    void openFiles();


};

#endif // PLAYLISTCONTROLLER_H
