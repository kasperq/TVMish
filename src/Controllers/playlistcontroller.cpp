#include "playlistcontroller.h"

#include <QQmlContext>

PlaylistController::PlaylistController(QQmlApplicationEngine &engine) : m_engine(&engine)
{
    qDebug() << "PlaylistController constructor: ";    
}

PlaylistController::~PlaylistController()
{
//    delete m_engine;
    m_engine = nullptr;
}

void PlaylistController::disconnectAllConnections()
{
    m_plGw.disconnect();
    m_plLists.disconnect();
    m_fileGW.disconnect();
    m_files.disconnect();
    m_channelGW.disconnect();
    m_channels.disconnect();
    m_categoryGW.disconnect();
    m_categories.disconnect();
}

void PlaylistController::openPlaylistManager()
{
    qDebug() << "openPlaylistManager";
    m_sets->getSets();
    initPlaylistConnections();
    openPlaylists(false);

    initFilesConnections();
    openFiles();    

    initChannelsConnections();
    openChannels();
}

void PlaylistController::openPlaylistViewer()
{
    qDebug() << "openPlaylistViewer";
    m_sets->getSets();
    initPlaylistConnections();
    openPlaylists(true);

    initCategoryConnections();
    openCategories();

    initChannelsConnections();
    openChannels();

    m_channelGW.select(m_curIdPlaylist, 0);
}

void PlaylistController::setSets(const Settings &value)
{
    m_sets = std::make_shared< Settings > (value);
}

void PlaylistController::addItemsFromDbToPlaylists(const int &idPlaylist)
{
    QString naim;
    bool isCurrent;
    int curIdPlaylist;
    uint num;

    m_curIdPlaylist = idPlaylist;

    m_channels.clear();
    m_files.clear();
    m_plLists.clear();

    if (m_plGw.rows() > 0) {
        m_plGw.data()->first();
        do {
            naim = m_plGw.data()->value("naim").toString();
            isCurrent = m_plGw.data()->value("is_current").toBool();
            curIdPlaylist = m_plGw.data()->value("id_playlist").toInt();
            num = m_plGw.data()->value("num").toUInt();

            m_plLists.addItem(naim, isCurrent, curIdPlaylist, num);
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

    m_channels.clear();
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

void PlaylistController::addItemsFromDbToChannels(const int &idPlaylist, const int &idFile)
{
    int idChannel;
    QString naim;
    QString url;
    int currentIdFile {idFile};
    int idCategory;
    int numPlaylist;
    int archDays;
    int idLogo;
//    int idPlaylist {m_plLists.curIdPlaylist()};
    QString catNaim;
    QUrl logoPath;
    bool isFavorite;
    int numFav;
    bool isAvailable;

    m_channels.clear();    
    m_channels.setIdPlaylist(idPlaylist);
    m_channels.setIdFile(idFile);

    if (m_channelGW.rows() > 0) {
        m_channelGW.data()->first();
        do {
            idChannel = m_channelGW.data()->value("id_channel").toInt();
            naim = m_channelGW.data()->value("naim").toString();
            url = m_channelGW.data()->value("url").toString();
            idCategory = m_channelGW.data()->value("id_category").toInt();
            numPlaylist = m_channelGW.data()->value("num_playlist").toInt();
            archDays = m_channelGW.data()->value("arch_days").toInt();
            idLogo = m_channelGW.data()->value("id_logo").toInt();

//            idPlaylist = m_channelGW.data()->value("id_playlist").toInt();
            currentIdFile = m_channelGW.data()->value("id_file").toInt();

            catNaim = m_channelGW.data()->value("category_naim").toString();
            logoPath = m_channelGW.data()->value("logo_path").toUrl();
            QString path = "file:/" + m_sets->appPath() + logoPath.toString();
            logoPath = QUrl(path);
            isFavorite = m_channelGW.data()->value("is_favorite").toBool();
            numFav = m_channelGW.data()->value("num_favorite").toInt();
            isAvailable = m_channelGW.data()->value("is_available").toBool();

            if (m_channels.getIdPlaylist() != idPlaylist)
                m_channels.setIdPlaylist(idPlaylist);
            if (m_channels.getIdFile() != idFile)
                m_channels.setIdFile(idFile);
            m_channels.addItem(idChannel, naim, url, currentIdFile, idPlaylist, idCategory, numPlaylist, archDays,
                               idLogo, catNaim, logoPath, isFavorite, numFav, isAvailable);
        } while(m_channelGW.data()->next());
    }    
    emit channelsAdded(idPlaylist, idFile);
}

void PlaylistController::addItemsFromDbToCategories()
{
    int idCategory {};
    QString catName {};
    bool isReadOnly {false};

    m_categories.clear();

    if (m_categoryGW.rows() > 0) {
        m_categoryGW.data()->first();
        do {
            idCategory = m_categoryGW.data()->value("id_category").toInt();
            catName = m_categoryGW.data()->value("name").toString();
            isReadOnly = m_categoryGW.data()->value("is_readonly").toBool();

            m_categories.addItem(idCategory, catName, isReadOnly);
        } while(m_categoryGW.data()->next());
    }
    emit categoriesAdded();
}

void PlaylistController::initPlaylistConnections()
{
    connect(&m_plGw, &PlaylistGW::selected, this, &PlaylistController::addItemsFromDbToPlaylists);
    connect(&m_plLists, SIGNAL(itemEdited(int,QString,bool,uint)), &m_plGw, SLOT(modify(int,QString,bool,uint)));

    connect(&m_plLists, SIGNAL(itemAdded(int,QString,bool)), &m_plGw, SLOT(insert(int,QString,bool)));
    connect(&m_plGw, SIGNAL(getNewIdPlaylistAndNum(int,int,uint)), &m_plLists, SLOT(setItemIdPlaylistAndNum(int,int,uint)));

    connect(&m_plLists, &Playlists::itemToDelete, &m_channelGW, &ChannelGW::deleteAllInPlaylist);   // first delete all channels
    connect(&m_plLists, &Playlists::itemToDelete, &m_fileGW, &PlFileGW::deleteAll);     // then delete all files
    connect(&m_plLists, &Playlists::itemToDelete, &m_plGw, &PlaylistGW::deleteRecord);  // then current playlist
    connect(&m_plGw, SIGNAL(deleted(int,int)), &m_plLists, SLOT(itemRemoved(int,int)));
}

void PlaylistController::openPlaylists(const bool &isCurrent)
{
    m_plGw.select(isCurrent);
    m_engine->rootContext()->setContextProperty(QStringLiteral("plLists"), &m_plLists);   
}

void PlaylistController::initFilesConnections()
{
    connect(&m_plLists, SIGNAL(playlistScrolled(int)), &m_fileGW, SLOT(select(int)));
    connect(&m_fileGW, SIGNAL(selected()), this, SLOT(addItemsFromDbToFiles()));
    connect(&m_fileGW, SIGNAL(selected()), &m_files, SIGNAL(listChanged()));
    connect(this, SIGNAL(filesAdded(int)), &m_files, SLOT(open(int)));

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
    m_files.initConnections();
    m_engine->rootContext()->setContextProperty(QStringLiteral("plFiles"), &m_files);
}

void PlaylistController::initChannelsConnections()
{
    connect(&m_files, &PlFiles::filesScrolled, &m_channelGW, &ChannelGW::select);
    connect(&m_channelGW, &ChannelGW::selected, this, &PlaylistController::addItemsFromDbToChannels);
    connect(&m_channelGW, &ChannelGW::selected, &m_channels, &Channels::listChanged);
    connect(this, &PlaylistController::channelsAdded, &m_channels, &Channels::open);
    connect(&m_channels, &Channels::openChannels, &m_channelGW, &ChannelGW::selectWithFilter);

    connect(&m_channels, &Channels::itemAppended, &m_channelGW, &ChannelGW::insert);
//    connect(&m_channels, &Channels::addForInsert, &m_channelGW, &ChannelGW::addForInsert);
    connect(&m_channelGW, &ChannelGW::inserted, &m_channels, &Channels::setNewIdChannelAndNums);

    connect(&m_channels, &Channels::getNums, &m_channelGW, &ChannelGW::getNums);
    connect(&m_channels, &Channels::saveChannels, &m_channelGW, &ChannelGW::save);

    connect(&m_channels, &Channels::itemEdited, &m_channelGW, &ChannelGW::modify);

    connect(&m_channels, &Channels::channelDeleted, &m_channelGW, &ChannelGW::deleteRecord);
    connect(&m_channelGW, &ChannelGW::deletedAllInFile, &m_channels, &Channels::deleteAllInFile);

    connect(&m_channels, &Channels::setFavorite, &m_channelGW, &ChannelGW::setFavorite);
    connect(&m_channelGW, &ChannelGW::sendFavoriteResult, &m_channels, &Channels::setFavoriteNum);

    connect(&m_channels, &Channels::insertLogo, &m_logosGW, &LogosGW::insert);
    connect(&m_logosGW, &LogosGW::inserted, &m_channels, &Channels::logoInserted);
    connect(&m_channels, &Channels::changeLogo, &m_logosGW, &LogosGW::modify);
    connect(&m_channels, &Channels::changeLogoPath, &m_logosGW, &LogosGW::modifyPath);
}

void PlaylistController::openChannels()
{
    m_channels.setSets(*m_sets);
    m_channels.initConnections();
    m_engine->rootContext()->setContextProperty(QStringLiteral("channels"), &m_channels);
}

void PlaylistController::initCategoryConnections()
{
    connect(&m_categoryGW, &CategoryGW::selected, this, &PlaylistController::addItemsFromDbToCategories);
    connect(&m_categoryGW, &CategoryGW::selected, &m_categories, &Categories::listChanged);
    connect(this, &PlaylistController::categoriesAdded, &m_categories, &Categories::open);    
}

void PlaylistController::openCategories()
{
    m_categoryGW.select();
    m_engine->rootContext()->setContextProperty(QStringLiteral("categories"), &m_categories);
}
