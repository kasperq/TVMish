#include "playlistcontroller.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

PlaylistController::PlaylistController(QQmlApplicationEngine &engine, DataBase &db) :
    m_engine(&engine),
    m_db(std::make_shared< DataBase > (db))
{
    qDebug() << "PlaylistController constructor: ";
    //    m_db = std::make_shared< DataBase > (db);
    m_sets = std::make_shared< Settings > (m_db->db());    
}

//PlaylistController::PlaylistController()
//{

//}

//PlaylistController &PlaylistController::operator=(PlaylistController &orig)
//{

//}

PlaylistController::~PlaylistController()
{
    m_plGwThread.quit();
    m_plGwThread.wait();

//    delete m_engine;
    m_engine = nullptr;
}

void PlaylistController::openPlaylistManager()
{
    qDebug() << "openPlaylistManager";
    initPlaylistConnections();
    openPlaylists();

    initFilesConnections();
    openFiles();
    qDebug() << "plContr: ";
}

void PlaylistController::addItemsFromDbToPlaylists()
{
    QString naim;
    bool isCurrent;
    int idPlaylist;
    uint num;

    m_plLists.clear();

    if (m_plGw.rows() > 0) {
        m_plGw.data()->first();
        do {
            naim = m_plGw.data()->value("naim").toString();
            isCurrent = m_plGw.data()->value("is_current").toBool();
            idPlaylist = m_plGw.data()->value("id_playlist").toInt();
            num = m_plGw.data()->value("num").toUInt();

            m_plLists.addItem(naim, isCurrent, idPlaylist, num);
        } while(m_plGw.data()->next());
    }
}

void PlaylistController::addItemsFromDbToFiles()
{
    QString fileName;
    QString filePath;
    QString filePathLocal;
    int idFile;
    int idFormat;
    bool isAvailable;
    QString format;
    int idPlaylist {m_plLists.curIdPlaylist()};

    m_files.clear();
    m_files.setIdPlaylist(idPlaylist);

    //adding default file/category "All"
//    if (idPlaylist == 0) {
//        fileName = tr("All");
//        filePath = filePathLocal = "";
//        idFile = 0;
//        this->addItem(&idFile, &idPlaylist, &fileName, &filePath, &filePathLocal, &idFormat, &isAvailable, &format);

    //adding files added manually
    if (m_fileGW.rows() > 0) {
        m_fileGW.data()->first();
        do {
            idPlaylist = m_fileGW.data()->value("id_playlist").toInt();
            idFile = m_fileGW.data()->value("id_file").toInt();
            fileName = m_fileGW.data()->value("file_name").toString();
            filePath = m_fileGW.data()->value("file_path").toString();
            filePathLocal = m_fileGW.data()->value("file_path_local").toString();
            idFormat = m_fileGW.data()->value("id_format").toInt();
            isAvailable = m_fileGW.data()->value("is_available").toBool();
            format = m_fileGW.data()->value("format").toString();

            m_files.setIdPlaylist(idPlaylist);
            m_files.addItem(idFile, idPlaylist, fileName, filePath, filePathLocal, idFormat, isAvailable, format);            
        } while(m_fileGW.data()->next());
    }    
    emit filesAdded(idPlaylist);
}

void PlaylistController::initPlaylistConnections()
{
    connect(&m_plGw, SIGNAL(selected()), this, SLOT(addItemsFromDbToPlaylists()));
    connect(&m_plLists, SIGNAL(itemEdited(int,QString,bool,uint)), &m_plGw, SLOT(modify(int,QString,bool,uint)));

    connect(&m_plLists, SIGNAL(itemAdded(int,QString,bool)), &m_plGwThread, SLOT(insertRecord(int,QString,bool)));
    connect(&m_plGwThread, SIGNAL(getNewIdPlaylistAndNum(int,int,uint)), &m_plLists, SLOT(setItemIdPlaylistAndNum(int,int,uint)));

    connect(&m_plLists, SIGNAL(itemToDelete(int,int)), &m_plGwThread, SLOT(deleteRecord(int,int)));
    connect(&m_plGwThread, SIGNAL(deleted(int,int)), &m_plLists, SLOT(itemRemoved(int,int)));
//    connect(&m_plGwThread, PlaylistGwThread::finished(), &m_plGwThread, QObject::deleteLater());
}

void PlaylistController::openPlaylists()
{
    m_plGw.select();
    m_engine->rootContext()->setContextProperty(QStringLiteral("plLists"), &m_plLists);   
    m_plGwThread.start();
}

void PlaylistController::initFilesConnections()
{
    connect(&m_plLists, SIGNAL(playlistScrolled(int)), &m_fileGW, SLOT(select(int)));
    connect(&m_fileGW, SIGNAL(selected()), this, SLOT(addItemsFromDbToFiles()));
    connect(&m_fileGW, SIGNAL(selected()), &m_files, SIGNAL(listChanged()));
//    connect(this, SIGNAL(filesAdded(int)), &m_files, SLOT(open(int)));

    connect(&m_files, SIGNAL(itemEdited(int,int,int,QString,QString,QString,int,bool)),
            &m_fileGW, SLOT(modify(int,int,int,QString,QString,QString,int,bool)));

    connect(&m_files, SIGNAL(itemDeleted(int,int)), &m_fileGW, SLOT(deleteRecord(int,int)));

    connect(&m_files, SIGNAL(itemAppended(int,int,QString,QString,QString,int,bool)),
            &m_fileGW, SLOT(insert(int,int,QString,QString,QString,int,bool)));
    connect(&m_fileGW, SIGNAL(inserted(int,int)), &m_files, SLOT(setNewItemIdFile(int,int)));
    connect(&m_files, SIGNAL(checkFileExtension(int,QString&,QString)), &m_fileGW, SLOT(checkFileExtension(int,QString&,QString)));
    connect(&m_fileGW, SIGNAL(extensionChecked(int,QString&,QString,bool,int)), &m_files, SLOT(extensionChecked(int,QString&,QString,bool,int)));

//    connect(&m_plLists, SIGNAL(playlistScrolled(int)), &m_files, SLOT(open(int)));
}

void PlaylistController::openFiles()
{
    m_files.setSets(*m_sets);
    m_engine->rootContext()->setContextProperty(QStringLiteral("plFiles"), &m_files);
}
