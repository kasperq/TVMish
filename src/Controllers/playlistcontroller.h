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

#include "Gateways/channelgw.h"
#include "Classes/channels.h"

#include "Classes/settings.h"

//#include "DB/database.h"

class PlaylistController : public QObject
{
    Q_OBJECT
public:
    PlaylistController(QQmlApplicationEngine &engine);
//    PlaylistController(QQmlApplicationEngine &engine, DataBase &db);
//    PlaylistController();
//    PlaylistController& operator=(PlaylistController &orig);
    virtual ~PlaylistController();

    void disconnectAllConnections();
    void openPlaylistManager();

signals:
    void filesAdded(const int &idPlaylist);
    void channelsAdded(const int &idPlaylist, const int &idFile);

public slots:
    void addItemsFromDbToPlaylists();
    void addItemsFromDbToFiles();
    void addItemsFromDbToChannels();

private:
    QQmlApplicationEngine *m_engine;
//    std::shared_ptr< DataBase > m_db;
    std::shared_ptr< Settings > m_sets;

    PlaylistGW m_plGw {this};
    Playlists m_plLists;

    PlFiles m_files;
    PlFileGW m_fileGW {this};

    ChannelGW m_channelGW;
    Channels m_channels;

    void initPlaylistConnections();
    void openPlaylists();

    void initFilesConnections();
    void openFiles();

    void initChannelsConnections();
    void openChannels();
};

#endif // PLAYLISTCONTROLLER_H
