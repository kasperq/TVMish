#include <QtQml/QQmlEngine>
#include <QQmlContext>


#include "maincontroller.h"

#include "Models/playlistsmodel.h"
#include "Models/plfilemodel.h"
#include "Models/channelsmodel.h"
#include "Models/categorymodel.h"

#include "Classes/playlists.h"
#include "Classes/plfiles.h"
#include "Classes/channels.h"
#include "Classes/categories.h"
#include "Classes/videoplayer.h"

MainController::MainController()
{

}

MainController::~MainController()
{
//    delete viewer;
}

void MainController::loadMainForm()
{
    openTvMode();

    qmlRegisterType<PlaylistsModel>("Playlists", 1, 0, "PlaylistsModel");
    qmlRegisterUncreatableType<Playlists>("Playlists", 1, 0, "Playlists",
                                          QStringLiteral("Playlists shouldn't be created in QML"));

    qmlRegisterType<PlFilesModel>("Playlists", 1, 0, "PlFilesModel");
    qmlRegisterUncreatableType<PlFiles>("Playlists", 1, 0, "PlFiles",
                                          QStringLiteral("PlFiles shouldn't be created in QML"));

    qmlRegisterType<ChannelsModel>("Playlists", 1, 0, "ChannelsModel");
    qmlRegisterUncreatableType<Channels>("Playlists", 1, 0, "Channels",
                                          QStringLiteral("Channels shouldn't be created in QML"));

    qmlRegisterType<CategoryModel>("Playlists", 1, 0, "CategoryModel");
    qmlRegisterUncreatableType<Categories>("Playlists", 1, 0, "Categories",
                                          QStringLiteral("Categoriess shouldn't be created in QML"));

    qmlRegisterUncreatableType<VideoPlayer>("Playlists", 1, 0, "_videoPlayer",
                                            QStringLiteral("VideoPlayer shouldn't be created in QML"));

    engine.load(QUrl(QStringLiteral("qrc:/Qml/MainForm.qml")));
    if (engine.rootObjects().isEmpty())
        qDebug() << "exit";

    engine.rootContext()->setContextProperty(QStringLiteral("mainContr"), this);
}

void MainController::openTvMode()
{    
    m_tvContr.disconnectAllConnections();
    m_tvContr.setSets(m_sets);
    m_tvContr.openTVMode();           
}

void MainController::openOptions()
{

}

void MainController::openPlaylistManager()
{
    m_plContr.disconnectAllConnections();
    m_plContr.setSets(m_sets);
    m_plContr.openPlaylistManager();
}

void MainController::openTvSchedule()
{

}

void MainController::openPlaylistChooser()
{

}
