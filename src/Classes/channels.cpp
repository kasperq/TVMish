#include "channels.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

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
            && item.numFile() == oldItem.numFile()
            && item.numPlaylist() == oldItem.numPlaylist()
            && item.archDays() == oldItem.archDays()
            && item.idLogo() == oldItem.idLogo())
        return false;

    m_channels[index] = item;
    int idChannel = item.idChannel();
    QString naim = item.naim();
    QString url = item.url();
    int idPlaylist = item.idPlaylist();
    int idFile = item.idFile();
    int idCategory = item.idCategory();
    int numFile = item.numFile();
    int numPlaylist = item.numPlaylist();
    int archDays = item.archDays();
    int idLogo = item.idLogo();

    emit itemEdited(index, idChannel, naim, url, idFile, idPlaylist, idCategory, numFile, numPlaylist, archDays, idLogo);
    return true;
}

void Channels::addItem(const int &idChannel, const QString &naim, const QString &url, const int &idFile,
                       const int &idPlaylist, const int &idCategory, const int &numFile, const int &numPlaylist,
                       const int &archDays, const int &idLogo)
{
    Channel newCh;
    newCh.setIdChannel(idChannel);
    newCh.setNaim(naim);
    newCh.setUrl(url);
    newCh.setIdFile(idFile);
    newCh.setIdPlaylist(idPlaylist);
    newCh.setIdCategory(idCategory);
    newCh.setNumFile(numFile);
    newCh.setNumPlaylist(numPlaylist);
    newCh.setArchDays(archDays);
    newCh.setIdLogo(idLogo);

    m_channels.append(newCh);
}

void Channels::clear()
{
    m_channels.clear();
}

int Channels::rowCount() const
{
    return m_channels.size();
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
    qDebug() << "channels: open: " << idFile;
}

//void Channels::parseFile(const QString &filePath, const int &idPlaylist, const int &idFile)
//{
//    qDebug() << "channels: parseFile: " << idPlaylist << " IdFile: " << idFile;
//    QFile file(filePath);
//    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        QTextStream fileStream(&file);
//        QString line = fileStream.readLine();
//        if (line.contains(m_format.begin_file, Qt::CaseInsensitive)) {
//            while (!fileStream.atEnd()) {
//                line = fileStream.readLine();
//                if (!line.isEmpty()) {
//                    parseLine(line);
//                    qDebug() << " line: " << line;
//                }
//            }
//        }
//    }
//}

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
    qDebug() << "channels: addParsedChannel: size: " << m_channelsToAdd.size() << " isPausedparsing: " << m_isPausedParsing;
    if (m_channelsToAdd.size() < m_parseLimit && !m_isPausedParsing) {
        m_isPausedParsing = false;
        emit pauseParsing(m_isPausedParsing);
        m_mutex.lock();
        m_channelsToAdd.push(newChannel); // std::queue

        qDebug() << "channels: added: " << newChannel.naim() << " front: " << m_channelsToAdd.front().naim();
        m_mutex.unlock();       
    } else {
        emit save();
        m_isPausedParsing = true;
        emit pauseParsing(m_isPausedParsing);

        m_channelsBuffer.push(newChannel);
        qDebug() << "channels: added buffer: " << newChannel.naim() << " front: " << m_channelsBuffer.front().naim();
    }
    checkQueuesAndParse();
}

void Channels::addChannel(/*const int &idPlaylist, const int &idFile*/)
{
    qDebug() << "channels: addChannel: size: " << m_channelsToAdd.size() << "isInsert: " << m_isInsertingChannel;
    if (m_channelsToAdd.size() > 0 && !m_isInsertingChannel) {
        m_isInsertingChannel = true;
        m_mutex.lock();
        auto newChannel = m_channelsToAdd.front();
        m_channelsToAdd.pop();
        m_mutex.unlock();

        int idPlaylist = newChannel.idPlaylist();
        int idFile = newChannel.idFile();
        qDebug() << "channels: addChannel: " << newChannel.naim();
        if (idPlaylist == m_curIdPlaylist && idFile == m_curIdFile) {
            m_channels.append(newChannel);
            bool genNums {true};

//            emit addForInsert(m_channels.size() - 1, newChannel.naim(), newChannel.url(), newChannel.idFile(), newChannel.idPlaylist(),
//                              newChannel.idCategory(), newChannel.numFile(), newChannel.numPlaylist(), newChannel.archDays(),
//                              newChannel.idLogo(), genNums);

            emit itemAppended(m_channels.size() - 1, newChannel.naim(), newChannel.url(), newChannel.idFile(), newChannel.idPlaylist(),
                              newChannel.idCategory(), newChannel.numFile(), newChannel.numPlaylist(), newChannel.archDays(),
                              newChannel.idLogo(), genNums);
        } else {
            qDebug() << "channels: addChannel: m_isInsertingChannel = false;";
            m_isInsertingChannel = false;
        }
    }
    if (m_channelsToAdd.size() == 0 && m_channelsBuffer.size() == 0 && m_isPausedParsing) {
        m_isPausedParsing = false;
        emit pauseParsing(m_isPausedParsing);
    }
}

void Channels::setNewIdChannelAndNums(const int &index, const int &newId, const int &numFile, const int &numPlaylist,
                                      const int &idFile, const int &idPlaylist)
{    
    auto newChannel = m_channels.at(index);
    if (newChannel.idFile() == idFile && newChannel.idPlaylist() == idPlaylist) {
        newChannel.setIdChannel(newId);
        newChannel.setNumFile(numFile);
        newChannel.setNumPlaylist(numPlaylist);
        qDebug() << "Channels: newId: " << newChannel.naim();
        setItemAt(index, newChannel);
    }
    m_isInsertingChannel = false;
//    qDebug() << "channels: setNewIdChannelAndNums: size: " << m_channelsToAdd.size() << " m_isInserting: " << m_isInsertingChannel
    checkQueuesAndParse();
//    if (m_channelsToAdd.size() > 0 && !m_isInsertingChannel)
//        emit channelParsed();
//    if (m_channelsToAdd.size() == 0 && m_channelsBuffer.size() == 0 && m_isPausedParsing) {
//        m_isPausedParsing = false;
//        emit pauseParsing(m_isPausedParsing);
//    }
//    if (m_channelsToAdd.size() < m_parseLimit)
//        emit pauseParsing(false);

    emit itemChanged(index);
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

        emit channelDeleted(idChannel);
        emit afterItemRemoved();

        if (m_channels.size() > 1 && curIndex == 0)
            emit selectItem(1);
        if (m_channels.size() > 0)
            emit selectItem(curIndex);
    }
}

void Channels::save()
{
    if (!m_isInsertingChannel)
        emit saveChannels();
}

//void Channels::proccessParsedChannel()
//{
//    if (m_channelsToAdd.size() > 0 && !m_isInsertingChannel) {
//        m_isInsertingChannel = true;
//        auto newChannel = m_channelsToAdd.dequeue();
//        int idPlaylist = newChannel.idPlaylist();
//        int idFile = newChannel.idFile();
//        m_channels.append(newChannel);
//        emit getNums(m_channels.size() - 1, idPlaylist, idFile);
//    }
//}

void Channels::initConnections()
{
//    connect(&m_parseThrd, &PlaylistParseThread::finished, &m_parseThrd, &QObject::deleteLater);
    connect(this, &Channels::pauseParsing, &m_parseThrd, &PlaylistParseThread::setPauseParsing);
    connect(&m_parseThrd, &PlaylistParseThread::channelParsed, this, &Channels::addParsedChannel);
    connect(this, &Channels::channelParsed, this, &Channels::addChannel);
    connect(&m_parseThrd, &PlaylistParseThread::fullyParsed, this, &Channels::save);
}

void Channels::checkQueuesAndParse()
{
    qDebug() << "Channels::checkQueuesAndParse(): m_channelsToAdd.size(): " << m_channelsToAdd.size() << " m_channelsBuffer.size(): " << m_channelsBuffer.size() << " isPaus: " << m_isPausedParsing;
    if (m_channelsToAdd.size() == 0 && m_channelsBuffer.size() > 0) {
        auto channel = m_channelsBuffer.front();
        m_mutex.lock();
        m_channelsToAdd.push(channel);
        m_channelsBuffer.pop();
        m_mutex.unlock();
    }
    if (m_channelsToAdd.size() == 0 && m_channelsBuffer.size() == 0 && m_isPausedParsing) {
        m_isPausedParsing = false;
        emit pauseParsing(m_isPausedParsing);
    }
    if (m_channelsToAdd.size() > 0 && !m_isInsertingChannel)
        emit channelParsed();
}

//void Channels::parseLine(QString &line)
//{
//    int idChannel {};
//    QString naim {};
//    QString url {};
//    int idCategory {};
//    int numFile {};
//    int numPlaylist {};
//    int archDays {};
//    int idLogo {};

//    if (line.startsWith(m_format.begin_channelInfo, Qt::CaseInsensitive)) {
//        channel.setIdChannel(idChannel);

//        channel.setIdFile(m_curIdFile);
//        channel.setIdPlaylist(m_curIdPlaylist);

//        channel.setIdCategory(idCategory);
//        channel.setIdLogo(idLogo);
//        channel.setArchDays(archDays);
//        channel.setNaim(naim);
//        channel.setNumFile(numFile);
//        channel.setNumPlaylist(numPlaylist);
//        channel.setUrl(url);
//    }

//    m_channels.append(channel);
//}

void Channels::setIdPlaylist(const int &newCurIdPlaylist)
{
    m_curIdPlaylist = newCurIdPlaylist;
}

void Channels::setIdFile(const int &newCurIdFile)
{
    m_curIdFile = newCurIdFile;
}
