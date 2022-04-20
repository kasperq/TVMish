#include "channels.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFileDialog>

#include "./DB/dbst.h"

Channels::Channels(QObject *parent)
    : QObject{parent}
{
    initConnections();
}

Channels::~Channels()
{
    m_parseThrd.quit();
    m_parseThrd.wait();
}

QVector<Channel> Channels::items() const
{
    return m_channels;
}

bool Channels::setItemAt(int index, Channel &item)
{
    if (index < 0 || index >= m_channels.size())
        return false;

    const Channel &oldItem = m_channels.at(index);
    if (item.idPlaylist() == oldItem.idPlaylist()
            && item.idFile() == oldItem.idFile()
            && item.idChannel() == oldItem.idChannel()
            && item.naim() == oldItem.naim()
            && item.url() == oldItem.url()
            && item.idCategory() == oldItem.idCategory()           
            && item.numPlaylist() == oldItem.numPlaylist()
            && item.archDays() == oldItem.archDays()
            && item.idLogo() == oldItem.idLogo()
            && item.isFavorite() == oldItem.isFavorite()
            && item.numFavorite() == oldItem.numFavorite()
            && item.isAvailable() == oldItem.isAvailable())
        return false;

    m_channels[index] = item;
    int idChannel = item.idChannel();
    QString naim = item.naim();
    QString url = item.url();
    int idPlaylist = item.idPlaylist();
    int idFile = item.idFile();
    int idCategory = item.idCategory();
    int numPlaylist = item.numPlaylist();
    int archDays = item.archDays();
    int idLogo = item.idLogo();
    int numFav = item.numFavorite();
    bool isFavorite = item.isFavorite();
    bool isAvailable = item.isAvailable();

    emit itemEdited(index, idChannel, naim, url, idFile, idPlaylist, idCategory, numPlaylist, archDays, idLogo,
                    isFavorite, numFav, isAvailable);
    return true;
}

void Channels::addItem(const int &idChannel, const QString &naim, const QString &url, const int &idFile,
                       const int &idPlaylist, const int &idCategory, const int &numPlaylist, const int &archDays,
                       const int &idLogo, const QString &catNaim, const QUrl &logoPath, const bool &isFavorite,
                       const int &numFav, const bool &isAvailable)
{
    Channel newCh;
    newCh.setIdChannel(idChannel);
    newCh.setNaim(naim);
    newCh.setUrl(url);
    newCh.setIdFile(idFile);
    newCh.setIdPlaylist(idPlaylist);
    newCh.setIdCategory(idCategory);    
    newCh.setNumPlaylist(numPlaylist);
    newCh.setArchDays(archDays);
    newCh.setIdLogo(idLogo);
    newCh.setCategoryNaim(catNaim);
    newCh.setLogoPath(logoPath);
    newCh.setIsFavorite(isFavorite);
    newCh.setNumFavorite(numFav);
    newCh.setIsAvailable(isAvailable);

    m_channels.append(newCh);
}

void Channels::clear()
{
    if (m_channels.size() > 0)
        m_channels.clear();
    emit rowCountChanged(rowCount());
}

int Channels::rowCount() const
{
    if (!m_channels.isEmpty())
        return m_channels.size();
    return 0;
}

int Channels::getIdPlaylist() const
{
    return m_curIdPlaylist;
}

int Channels::getIdFile() const
{
    return m_curIdFile;
}

void Channels::open(const int &idPlaylist, const int &idFile)
{
    m_curIdFile = idFile;
    m_curIdPlaylist = idPlaylist;
    if (m_channels.size() > 0)
        emit selectItem(0);
    emit rowCountChanged(rowCount());    
    emit isAllFilesSelectedChanged(isAllFilesSelected());
}

void Channels::scroll(int index)
{
    if (m_channels.isEmpty())
        m_curIdChannel = 0;
    else
        m_curIdChannel = m_channels.at(index).idChannel();
    emit channelsScrolled(m_curIdChannel);
}

void Channels::update(const QString &filePath)
{
    qDebug() << "Channels: update: " << filePath;
    if (m_channels.size() > 0) {
        clear();
    }
//    parseFile(filePath, m_curIdPlaylist, m_curIdFile);
    m_parseThrd.prepareParsing(filePath, m_curIdPlaylist, m_curIdFile);    
    m_parseThrd.start();
}

void Channels::addParsedChannel(Channel &newChannel, const int &idPlaylist, const int &idFile)
{
    qDebug() << "Channels::addParsedChannel: " << newChannel.naim() << " m_isInserting: " << m_isInsertingChannel;
    if (!m_isInsertingChannel && newChannel.url() != m_newChannel.url()) {
//        m_parseThrd.setPauseParsing(true);
//        emit pauseParsing(true);
        m_isInsertingChannel = true;
        m_newChannel = newChannel;
        m_timer.setInterval(5000);
        m_timer.start();

        bool genNums {true};
        bool isFavorite {false};
        int numFav {};
        bool isAvailable {true};
        m_newChannel.setNumFavorite(numFav);
        m_newChannel.setIsAvailable(isAvailable);
        m_newChannel.setIsFavorite(isFavorite);

        if (idFile == m_curIdFile || (m_curIdFile == 0 && idPlaylist == m_curIdPlaylist))
            m_channels.append(m_newChannel);

        m_curAttempt = 1;
        qDebug() << "m_channels.size() - 1" << m_channels.size() - 1;
        emit itemAppended(m_channels.size() - 1, m_newChannel.naim(), m_newChannel.url(), m_newChannel.idFile(),
                          m_newChannel.idPlaylist(), m_newChannel.idCategory(), m_newChannel.numPlaylist(),
                          m_newChannel.archDays(), m_newChannel.idLogo(), genNums, m_newChannel.isFavorite(),
                          m_newChannel.numFavorite(), m_newChannel.isAvailable());
    }
}

void Channels::setNewIdChannelAndNums(const int &index, const int &newId, const int &numPlaylist, const int &idFile,
                                      const int &idPlaylist)
{    
    if (m_timer.isActive()) {
        m_timer.stop();
    }
    m_curAttempt = 0;
    if (idFile == m_curIdFile) {
        qDebug() << "Channels::setNewIdChannelAndNums: " << newId;
        auto newChannel = m_channels.at(index);
        if (newChannel.idFile() == idFile && newChannel.idPlaylist() == idPlaylist) {
            newChannel.setIdChannel(newId);
            newChannel.setNumPlaylist(numPlaylist);

            m_channels[index] = newChannel;
        }
        emit itemChanged(index);
    }
//    emit pauseParsing(false);
    m_isInsertingChannel = false;    
    emit channelAdded();
}

void Channels::restartInserting()
{
    m_timer.stop();
    qDebug() << "Channels::restartInserting()";
    DBst::getInstance().close().then([this](bool closeResult) {
        if (closeResult) {
            DBst::getInstance().open().then([this](bool openResult) {
                if (openResult) {
                    qDebug() << "Channels::restartInserting(): restarted DB";
                    if (m_curAttempt == m_maxAttempts) {
                        m_isInsertingChannel = false;
                        m_channels.pop_back();
                        emit channelAdded();
                    } else {
                        m_timer.start();
                        bool genNums {true};
                        ++m_curAttempt;
                        emit itemAppended(m_channels.size() - 1, m_newChannel.naim(), m_newChannel.url(), m_newChannel.idFile(),
                                          m_newChannel.idPlaylist(), m_newChannel.idCategory(), m_newChannel.numPlaylist(),
                                          m_newChannel.archDays(), m_newChannel.idLogo(), genNums, m_newChannel.isFavorite(),
                                          m_newChannel.numFavorite(), m_newChannel.isAvailable());
                    }
                }
            });
        }
    });
}

void Channels::deleteChannel(const int &index)
{
    if (index > -1 && index < m_channels.size()) {
        emit beforeItemRemoved(index);

        int curIndex;
        if (index == m_channels.size() - 1)
            curIndex = m_channels.size() - 2;
        else
            curIndex = index;

        auto ch = m_channels.at(index);
        int idChannel = ch.idChannel();
        m_channels.removeAt(index);

        emit channelDeleted(index, idChannel);
        emit afterItemRemoved();

        if (m_channels.size() > 1 && curIndex == 0)
            emit selectItem(1);
        if (m_channels.size() > 0)
            emit selectItem(curIndex);
    }
}

void Channels::deleteAllInFile(const int &idFile)
{
    m_channels.clear();
    emit rowCountChanged(rowCount());
}

void Channels::move(int index, QString type)
{    
    qsizetype curIndex;
    qsizetype moveToIndex;
    bool moving = false;

    if (index != -1) {
        if (type.toLower() == "up") {
            if (index > 0) {
                curIndex = index;
                moveToIndex = --index;
                moving = true;
            }
        } else {
            if (index < m_channels.size() - 1) {
                curIndex = index;
                moveToIndex = ++index;
                moving = true;
            }
        }
    }

    if (moving) {
        auto channel = m_channels.at(curIndex);
        auto ch2 = m_channels.at(moveToIndex);

        if (!channel.isFavorite() && !ch2.isFavorite()) {
            int moveToNum = ch2.numPlaylist();
            int curIndexNum = channel.numPlaylist();

            channel.setNumPlaylist(moveToNum);
            setItemAt(curIndex, channel);

            ch2.setNumPlaylist(curIndexNum);
            setItemAt(moveToIndex, ch2);

            m_channels.swapItemsAt(curIndex, moveToIndex);

            emit itemChanged(moveToIndex);
            emit selectItem(moveToIndex);
        }
        if (channel.isFavorite() && ch2.isFavorite()) {
            int moveToNum = ch2.numFavorite();
            int curIndexNum = channel.numFavorite();

            channel.setNumFavorite(moveToNum);
            setItemAt(curIndex, channel);

            ch2.setNumFavorite(curIndexNum);
            setItemAt(moveToIndex, ch2);

            m_channels.swapItemsAt(curIndex, moveToIndex);

            emit itemChanged(moveToIndex);
            emit selectItem(moveToIndex);
        }
    }
}

void Channels::setIsFavorite(int index, bool isFav)
{
    qDebug() << "CHannels: setIsFavorite: " << index << " isFav: " << isFav;
    auto channel = m_channels.at(index);
    channel.setIsFavorite(isFav);
    const int id = channel.idChannel();

    int i {};
    if (isFav) {
        for(i = index; i > 0; --i) {
            if (m_channels.at(i - 1).isFavorite())
                break;
        }
        m_channels.remove(index, 1);
        m_channels.insert(i, channel);
    } else {
        for(i = index; i < m_channels.size() - 1; ++i) {
            if (!m_channels.at(i + 1).isFavorite() && m_channels.at(i + 1).numPlaylist() > channel.numPlaylist())
                break;
        }
        channel.setNumFavorite(0);
        m_channels.remove(index, 1);
        m_channels.insert(i, channel);
    }
    emit setFavorite(i, isFav, id, m_curIdPlaylist);
    emit listChanged();
    emit selectItem(index);
}

void Channels::setFavoriteNum(const int &index, const int &idChannel, const int &num)
{
    qDebug() << "Channels::setFavoriteNum: id: " << idChannel << " num: " << num;
    int i {};
    if (m_channels.at(index).idChannel() == idChannel) {
        i = index;
    } else {
        for (i = 0; i < m_channels.size() - 1; ++i) {
            if (m_channels.at(i).idChannel() == idChannel)
                break;
        }
    }
    auto channel = m_channels.at(i);
    channel.setNumFavorite(num);
    m_channels[i] = channel;
}

void Channels::filter(QString text)
{
    m_filterNaim = text;
    emit openChannels(m_curIdPlaylist, m_curIdFile, m_filterNaim, m_curIdChannel);
}

void Channels::filterCategory(const int &idCategory)
{
    m_curIdCategory = idCategory;
    if (idCategory == 1)
        m_curIdCategory = 0;
    emit openChannels(m_curIdPlaylist, m_curIdFile, m_filterNaim, m_curIdCategory);
}

void Channels::find(QString text)
{
    int index {};
    for (index  = 0; index < m_channels.size() - 1; ++index)
        if (m_channels.at(index).naim().contains(text, Qt::CaseInsensitive))
            break;
    emit selectItem(index);
}

void Channels::setLogo(int index, QUrl filePath)
{
    QString newFilePath {m_sets->appPath() + m_sets->logoPath() + filePath.fileName()};
    m_plThr.copyFile(index, filePath, newFilePath, false);
    m_plThr.start();
}

void Channels::logoCopied(const QString &fileName, const QString &filePath, const QString &newFilePath,
                          const QString &extension, const int &idFormat, const bool &isAvailable, const int &index)
{
    qDebug() << "fileName: " << fileName << " filePath: " << filePath << " newFilePath: " << newFilePath << " ext: " << extension
             << " idFormat: " << idFormat << " isAva: " << isAvailable << " index: " << index;
    auto id = m_channels.at(index).idLogo();
    QString channelName = m_channels.at(index).naim();
    QString logoPathFull = newFilePath;
    QString logoPath = logoPathFull;
    logoPath.remove(m_sets->appPath());
    if (id <= 1) {
        emit insertLogo(index, channelName, logoPath, logoPath, channelName);
    } else {
        QFile file(newFilePath);
        QFileInfo fInf(newFilePath);
        QString fileExt = fInf.suffix();
        logoPathFull = m_sets->appPath() + m_sets->logoPath() + channelName + "_" + QString::number(id) + "." + fileExt;
        logoPath = logoPathFull;
        logoPath.remove(m_sets->appPath());
        if (!file.rename(logoPathFull)) {
            logoPathFull = newFilePath;
            logoPath = logoPathFull;
            logoPath.remove(m_sets->appPath());
        }

        emit changeLogo(index, id, channelName, logoPath, logoPath, channelName);

        auto channel = m_channels.at(index);
        channel.setLogoPath(QUrl("file:/" + logoPathFull));
        setItemAt(index, channel);

        emit itemChanged(index);
        emit selectItem(index);
        emit logoChanged(index, QUrl::fromLocalFile(logoPathFull));
    }
}

void Channels::logoInserted(const int &index, const int &newId, const QString &logoPath)
{
    auto channel = m_channels.at(index);

    QString channelName = channel.naim();
    QString logoPathFull = m_sets->appPath() + logoPath;
    QString logoPathShort = logoPath;

    QFile file(logoPathFull);
    QFileInfo fInf(logoPathFull);
    QString fileExt = fInf.suffix();
    logoPathFull = m_sets->appPath() + m_sets->logoPath() + channelName + "_" + QString::number(newId) + "." + fileExt;
    logoPathShort = logoPathFull;
    logoPathShort.remove(m_sets->appPath());
    if (!file.rename(logoPathFull)) {
        logoPathFull = m_sets->appPath() + logoPath;
        logoPathShort = logoPath;
    } else {
        channel.setLogoPath(QUrl("file:/" + logoPathFull));
        emit changeLogoPath(newId, logoPathShort, logoPathShort);
    }

    channel.setIdLogo(newId);
    setItemAt(index, channel);

    emit itemChanged(index);
    emit selectItem(index);
    emit logoChanged(index, QUrl::fromLocalFile(logoPathFull));
}

void Channels::save(const int &idPlaylist, const int &idFile)
{
    m_isSaving = true;
    if (!m_isInsertingChannel) {
        m_isSaving = false;
        emit saveChannels();
        emit channelsAdded(true);
        if (idFile == m_curIdFile || (m_curIdFile == 0 && idPlaylist == m_curIdPlaylist))
            emit openChannels(m_curIdPlaylist, m_curIdFile, m_filterNaim, m_curIdCategory);
    }
}

void Channels::initConnections()
{
//    connect(&m_parseThrd, &PlaylistParseThread::finished, &m_parseThrd, &QObject::deleteLater);
    connect(&m_timer, &QTimer::timeout, this, &Channels::restartInserting);
    connect(this, &Channels::pauseParsing, &m_parseThrd, &PlaylistParseThread::setPauseParsing);
    connect(this, &Channels::channelAdded, &m_parseThrd, &PlaylistParseThread::deleteFrontChannel);
    connect(&m_parseThrd, &PlaylistParseThread::channelParsed, this, &Channels::addParsedChannel);
    connect(&m_parseThrd, &PlaylistParseThread::fullyParsed, this, &Channels::save);
    connect(&m_plThr, &FileCopyThread::fileCopied, this, &Channels::logoCopied);
}

void Channels::setIdPlaylist(const int &newCurIdPlaylist)
{
    m_curIdPlaylist = newCurIdPlaylist;
}

bool Channels::isAllFilesSelected() const
{
    if (m_curIdFile == 0)
        return true;
    else
        return false;
}

void Channels::setSets(const Settings &value)
{
    m_sets = std::make_shared< Settings > (value);
}

void Channels::setIdFile(const int &newCurIdFile)
{
    m_curIdFile = newCurIdFile;
}
