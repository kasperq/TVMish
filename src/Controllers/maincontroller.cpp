#include <QtQml/QQmlEngine>
#include <QQmlContext>


#include "maincontroller.h"

#include "Models/playlistsmodel.h"
#include "Models/plfilemodel.h"
#include "Classes/playlists.h"
#include "Classes/plfiles.h"

MainController::MainController(QSqlDatabase db) : m_db(db)
{

}

//MainController::MainController()
//{

//}

MainController::MainController(DataBase &datab)
{
    m_dataB = std::make_shared< DataBase > (datab);
}

MainController::~MainController()
{
//    delete viewer;
}

void MainController::loadMainForm()
{
    qmlRegisterType<PlaylistsModel>("Playlists", 1, 0, "PlaylistsModel");
    qmlRegisterUncreatableType<Playlists>("Playlists", 1, 0, "Playlists",
                                          QStringLiteral("Playlists shouldn't be created in QML"));

    qmlRegisterType<PlFilesModel>("Playlists", 1, 0, "PlFilesModel");
    qmlRegisterUncreatableType<PlFiles>("Playlists", 1, 0, "PlFiles",
                                          QStringLiteral("PlFiles shouldn't be created in QML"));

    engine.load(QUrl(QStringLiteral("qrc:/Qml/MainForm.qml")));
    if (engine.rootObjects().isEmpty())
        qDebug() << "exit";

    engine.rootContext()->setContextProperty(QStringLiteral("mainContr"), this);


//    const QUrl url(QStringLiteral("qrc:/Forms/MainForm.qml"));
//    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
//                     m_app, [url](QObject *obj, const QUrl &objUrl) {
//        if (!obj && url == objUrl) {
//            qDebug() << "exit app";
//            QCoreApplication::exit(-1);
//        }
//        }, Qt::QueuedConnection);
//    engine.load(url);

    //    viewer = new QQuickView();
////    connect(viewer->engine(), &QQmlEngine::quit, &viewer, &QWindow::close);
//    viewer->setTitle(QStringLiteral("TVmish"));

////    viewer->rootContext()->setContextProperty("dataSource", &dataSource);
////    viewer->rootContext()->setContextProperty("openGLSupported", openGLSupported);

//    viewer->setSource(QUrl("qrc:/Forms/MainForm.qml"));
//    viewer->setResizeMode(QQuickView::SizeRootObjectToView);
////    viewer->setColor(QColor("#404040"));
    //    viewer->show();
}

//void MainController::setDb(DataBase &db)
//{
//    m_dataB = std::make_shared< DataBase > (db);
//}

void MainController::openTvMode()
{

}

void MainController::openOptions()
{

}

void MainController::openPlaylistManager()
{
//    qDebug() << "opening plmanager";
//    plContr = new PlaylistController(&engine, m_db);
//    plContr = PlaylistController(engine, *m_dataB);
//    plContr = PlaylistController(new PlaylistController(engine, *m_dataB));
    plContr.openPlaylistManager();
}

void MainController::openTvSchedule()
{

}

void MainController::openPlaylistChooser()
{

}
