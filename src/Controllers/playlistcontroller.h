#ifndef PLAYLISTCONTROLLER_H
#define PLAYLISTCONTROLLER_H

#include <QObject>
#include <QQmlApplicationEngine>

#include "Gateways/playlistgw.h"
#include "Classes/playlists.h"

#include "Gateways/plfilegw.h"
#include "Classes/plfiles.h"

class PlaylistController : public QObject
{
    Q_OBJECT
public:
    explicit PlaylistController(QQmlApplicationEngine *engine, QSqlDatabase db);
    virtual ~PlaylistController();

    void openPlaylistManager();

signals:

private:
    QQmlApplicationEngine *m_engine;
    QSqlDatabase m_db;

    Playlists *m_plLists;
    PlaylistGW *m_plGw;

    PlFiles *m_files;
    PlFileGW *m_fileGW;


};

#endif // PLAYLISTCONTROLLER_H
