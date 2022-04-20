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
#include "Gateways/logosgw.h"
#include "Classes/categories.h"
#include "Gateways/categorygw.h"

#include "Classes/settings.h"


class PlaylistController : public QObject
{
    Q_OBJECT
public:
    PlaylistController(QQmlApplicationEngine &engine);
    virtual ~PlaylistController();

    void disconnectAllConnections();
    void openPlaylistManager();
    void openPlaylistViewer();

    void setSets(const Settings &value);

signals:
    void filesAdded(const int &idPlaylist);
    void channelsAdded(const int &idPlaylist, const int &idFile);
    void categoriesAdded();

public slots:
    void addItemsFromDbToPlaylists(const int &idPlaylist);
    void addItemsFromDbToFiles();
    void addItemsFromDbToChannels(const int &idPlaylist, const int &idFile);
    void addItemsFromDbToCategories();

private:
    QQmlApplicationEngine *m_engine;

    std::shared_ptr< Settings > m_sets;

    PlaylistGW m_plGw {this};
    Playlists m_plLists;

    PlFiles m_files;
    PlFileGW m_fileGW {this};

    ChannelGW m_channelGW;
    Channels m_channels;
    LogosGW m_logosGW;
    CategoryGW m_categoryGW;
    Categories m_categories;

    int m_curIdPlaylist {};

    void initPlaylistConnections();
    void openPlaylists(const bool &isCurrent);

    void initFilesConnections();
    void openFiles();

    void initChannelsConnections();
    void openChannels();

    void initCategoryConnections();
    void openCategories();
};

#endif // PLAYLISTCONTROLLER_H
