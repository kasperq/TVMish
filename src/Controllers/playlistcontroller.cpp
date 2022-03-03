#include "playlistcontroller.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

PlaylistController::PlaylistController(QQmlApplicationEngine &engine) : m_engine(&engine)
{
    qDebug() << "PlaylistController constructor: ";
    m_sets = std::make_shared< Settings > ();
}

//PlaylistController::PlaylistController(QQmlApplicationEngine &engine, DataBase &db) :
//    m_engine(&engine),
//    m_db(std::make_shared< DataBase > (db))
//{
//    qDebug() << "PlaylistController constructor: ";
//    //    m_db = std::make_shared< DataBase > (db);
//    m_sets = std::make_shared< Settings > (/*m_db->db()*/);
//}

//PlaylistController::PlaylistController()
//{

//}

//PlaylistController &PlaylistController::operator=(PlaylistController &orig)
//{

//}

PlaylistController::~PlaylistController()
{
//    delete m_engine;
    m_engine = nullptr;
}

void PlaylistController::disconnectAllConnections()
{
    m_plGw.disconnect();
    m_plLists.disconnect();
}

void PlaylistController::openPlaylistManager()
{
    qDebug() << "openPlaylistManager";
    initPlaylistConnections();
    openPlaylists();

    initFilesConnections();
    openFiles();
    qDebug() << "plContr: ";

    initChannelsConnections();
    openChannels();
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

            if (m_files.idPlaylist() != idPlaylist)
                m_files.setIdPlaylist(idPlaylist);
            m_files.addItem(idFile, idPlaylist, fileName, filePath, filePathLocal, idFormat, isAvailable, format);            
        } while(m_fileGW.data()->next());
    }    
    emit filesAdded(idPlaylist);
}

void PlaylistController::addItemsFromDbToChannels()
{
    int idChannel;
    QString naim;
    QString url;
    int idFile {m_files.curIdFile()};
    int idCategory;
    int numFile;
    int numPlaylist;
    int archDays;
    int idLogo;
    int idPlaylist {m_plLists.curIdPlaylist()};

    m_channels.clear();    
    m_channels.setIdPlaylist(idPlaylist);
    m_channels.setIdFile(idFile);

    if (m_channelGW.rows() > 0) {
        m_channelGW.data()->first();
        do {
            idChannel = m_channelGW.data()->value("id_channel").toInt();;
            naim = m_channelGW.data()->value("naim").toString();
            url = m_channelGW.data()->value("url").toString();
            idCategory = m_channelGW.data()->value("id_category").toInt();;
            numFile = m_channelGW.data()->value("num_file").toInt();;
            numPlaylist = m_channelGW.data()->value("num_playlist").toInt();;
            archDays = m_channelGW.data()->value("arch_days").toInt();;
            idLogo = m_channelGW.data()->value("id_logo").toInt();;

            idPlaylist = m_channelGW.data()->value("id_playlist").toInt();
            idFile = m_channelGW.data()->value("id_file").toInt();

            if (m_channels.getIdPlaylist() != idPlaylist)
                m_channels.setIdPlaylist(idPlaylist);
            if (m_channels.getIdFile() != idFile)
                m_channels.setIdFile(idFile);
            m_channels.addItem(idChannel, naim, url, idFile, idPlaylist, idCategory, numFile, numPlaylist, archDays, idLogo);
        } while(m_channelGW.data()->next());
    }
    qDebug() << "PlaylistController::addItemsFromDbToChannels()";
    emit channelsAdded(idPlaylist, idFile);
}

void PlaylistController::initPlaylistConnections()
{
    connect(&m_plGw, SIGNAL(selected()), this, SLOT(addItemsFromDbToPlaylists()));
    connect(&m_plLists, SIGNAL(itemEdited(int,QString,bool,uint)), &m_plGw, SLOT(modify(int,QString,bool,uint)));

    connect(&m_plLists, SIGNAL(itemAdded(int,QString,bool)), &m_plGw, SLOT(insert(int,QString,bool)));
    connect(&m_plGw, SIGNAL(getNewIdPlaylistAndNum(int,int,uint)), &m_plLists, SLOT(setItemIdPlaylistAndNum(int,int,uint)));

    connect(&m_plLists, &Playlists::itemToDelete, &m_channelGW, &ChannelGW::deleteAllInPlaylist);   // first delete all channels
    connect(&m_plLists, &Playlists::itemToDelete, &m_fileGW, &PlFileGW::deleteAll);     // then delete all files
    connect(&m_plLists, &Playlists::itemToDelete, &m_plGw, &PlaylistGW::deleteRecord);  // then current playlist
    connect(&m_plGw, SIGNAL(deleted(int,int)), &m_plLists, SLOT(itemRemoved(int,int)));
}

void PlaylistController::openPlaylists()
{
    m_plGw.select();
    m_engine->rootContext()->setContextProperty(QStringLiteral("plLists"), &m_plLists);   
}

void PlaylistController::initFilesConnections()
{
    connect(&m_plLists, SIGNAL(playlistScrolled(int)), &m_fileGW, SLOT(select(int)));
    connect(&m_fileGW, SIGNAL(selected()), this, SLOT(addItemsFromDbToFiles()));
    connect(&m_fileGW, SIGNAL(selected()), &m_files, SIGNAL(listChanged()));
    connect(this, SIGNAL(filesAdded(int)), &m_files, SLOT(open(int)));
//    connect(this, SIGNAL(filesAdded(int)), &m_files, SLOT(open(int)));

    connect(&m_files, SIGNAL(itemEdited(int,int,int,QString,QString,QString,int,bool)),
            &m_fileGW, SLOT(modify(int,int,int,QString,QString,QString,int,bool)));

    connect(&m_files, &PlFiles::itemDeleted, &m_channelGW, &ChannelGW::deleteAllInFile);      // first delete all channels
    connect(&m_files, SIGNAL(itemDeleted(int,int)), &m_fileGW, SLOT(deleteRecord(int,int)));    // then delete current file

    connect(&m_files, SIGNAL(itemAppended(int,int,QString,QString,QString,int,bool)),
            &m_fileGW, SLOT(insert(int,int,QString,QString,QString,int,bool)));
    connect(&m_fileGW, SIGNAL(inserted(int,int)), &m_files, SLOT(setNewItemIdFile(int,int)));
    connect(&m_files, SIGNAL(checkFileExtension(int,QString&,QString&,QString,uint)),
            &m_fileGW, SLOT(checkFileExtension(int,QString&,QString&,QString,uint)));
    connect(&m_fileGW, SIGNAL(extensionChecked(int,QString&,QString&,QString,bool,int,uint)),
            &m_files, SLOT(extensionChecked(int,QString&,QString&,QString,bool,int,uint)));
}

void PlaylistController::openFiles()
{
    m_files.setSets(*m_sets);
    m_engine->rootContext()->setContextProperty(QStringLiteral("plFiles"), &m_files);
}

void PlaylistController::initChannelsConnections()
{
    connect(&m_files, &PlFiles::filesScrolled, &m_channelGW, &ChannelGW::select);
    connect(&m_channelGW, &ChannelGW::selected, this, &PlaylistController::addItemsFromDbToChannels);
    connect(&m_channelGW, &ChannelGW::selected, &m_channels, &Channels::listChanged);
    connect(this, &PlaylistController::channelsAdded, &m_channels, &Channels::open);

    connect(&m_channels, &Channels::itemAppended, &m_channelGW, &ChannelGW::insert);
    connect(&m_channels, &Channels::addForInsert, &m_channelGW, &ChannelGW::addForInsert);
    connect(&m_channelGW, &ChannelGW::inserted, &m_channels, &Channels::setNewIdChannelAndNums);

    connect(&m_channels, &Channels::getNums, &m_channelGW, &ChannelGW::getNums);
//    connect(&m_channels, &Channels::saveChannels, &m_channelGW, &ChannelGW::saveInsert);
    connect(&m_channels, &Channels::saveChannels, &m_channelGW, &ChannelGW::save);
}

void PlaylistController::openChannels()
{
    m_engine->rootContext()->setContextProperty(QStringLiteral("channels"), &m_channels);
}
