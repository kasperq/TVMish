#include "playlistcontroller.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

PlaylistController::PlaylistController(QQmlApplicationEngine *engine, QSqlDatabase db) : m_engine(engine), m_db(db)
{

}

PlaylistController::~PlaylistController()
{
    delete m_engine;
    m_engine = nullptr;

    delete m_plLists;
    m_plLists = nullptr;
    delete m_plGw;
    m_plGw = nullptr;

    delete m_files;
    m_files = nullptr;
    delete m_fileGW;
    m_fileGW = nullptr;
}

void PlaylistController::openPlaylistManager()
{
    QString naim;
    bool isCurrent;
    int idPlaylist;
    uint num;

//    qmlRegisterType<PlaylistsModel>("Playlists", 1, 0, "PlaylistsModel");
//    qmlRegisterUncreatableType<Playlists>("Playlists", 1, 0, "Playlists",
//                                          QStringLiteral("Playlists shouldn't be created in QML"));
    m_plGw = new PlaylistGW(m_db);
    m_plLists = new Playlists(m_plGw);
//    plModel = new PlaylistsModel(plGw, plLists);
    m_plGw->select();
    if (*m_plGw->rows() > 0) {
        m_plGw->data()->first();
        do {
            naim = m_plGw->data()->value("naim").toString();
            isCurrent = m_plGw->data()->value("is_current").toBool();
            idPlaylist = m_plGw->data()->value("id_playlist").toInt();
            num = m_plGw->data()->value("num").toUInt();

            m_plLists->addItem(&naim, &isCurrent, &idPlaylist, &num);
        } while(m_plGw->data()->next());
    }
//    plModel = new PlaylistsModel(plGw, plLists);
//    plModel = new PlaylistsModel(plGw);
//    plModel->setPlLists(plLists);
//    m_engine->rootContext()->setContextProperty(QStringLiteral("plModel"), plModel);
    m_engine->rootContext()->setContextProperty(QStringLiteral("plLists"), m_plLists);

    m_fileGW = new PlFileGW();
    m_files = new PlFiles(m_fileGW);
    m_engine->rootContext()->setContextProperty(QStringLiteral("plFiles"), m_files);
    QObject::connect(m_plLists, SIGNAL(playlistScrolled(int)), m_files, SLOT(open(int)));
//    m_fileGW->select(plLists.

}
