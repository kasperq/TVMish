#include "videoplayer.h"

#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QDebug>
#include <QDir>

#include <QtMultimedia/QMediaMetaData>

VideoPlayer::VideoPlayer(QObject *parent)
    : QObject{parent}
{    
    initConnections();
}

VideoPlayer::~VideoPlayer()
{

}

void VideoPlayer::initConnections()
{
    connect(&m_fileLoader, &FileDownloader::error, this, &VideoPlayer::channelUnavailable);
    connect(&m_fileLoader, &FileDownloader::fileDownloaded, this, &VideoPlayer::parseChannelPlaylist);
//    connect(&m_timer, &QTimer::timeout, this, &VideoPlayer::play);
    connect(&m_timer, &QTimer::timeout, this, [=]() {
        m_timer.stop();
        play();
    });

    connect(&m_chainLoader, &FileDownloader::fileDownloaded, this, &VideoPlayer::channelChainDownloaded);
}

void VideoPlayer::setSets(const Settings &value)
{
    m_sets = std::make_shared< Settings > (value);
}

void VideoPlayer::playChannel(QUrl &channelUrl, const bool &fromBeginning)
{
    qDebug() << "VideoPlayer::playChannel: " << channelUrl;
    m_curChannelUrl = QUrl(channelUrl);
    if (fromBeginning) {
        emit stopPlaying();
        QDir dir(m_sets->appPath() + "/temp/current_channel/");
        dir.setNameFilters(QStringList() << "*.*");
        dir.setFilter(QDir::Files);
        foreach(QString dirFile, dir.entryList())
            dir.remove(dirFile);
        if (!m_channelPlaylist.isEmpty())
            m_channelPlaylist.clear();
    }
    downLoadChannelM3UFile(channelUrl);

}

void VideoPlayer::parseChannelPlaylist(const QString &fileName, const QString &filePath, const QString &newFilePath,
                                       const QString &extension, const int &idFormat, const bool &isAvailable, const int &index)
{
    qDebug() << "VideoPlayer::parseChannelPlaylist: " << fileName;
    QFile file(newFilePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream fileStream(&file);
        m_curSegmentFileName = getSegmentName();
        QString line = fileStream.readLine();
        if (line.contains(m_format.begin_file, Qt::CaseInsensitive)) {
            while (!fileStream.atEnd()) {
                line = fileStream.readLine();
                qDebug() << "line: " << line;
                if (!line.isEmpty()) {
                    parseLine(line);
                }
            }
        }
    }
//    play();
    downloadChannelChain(false);
}

void VideoPlayer::channelUnavailable(const QString &msg, const QString &fileName, const QString &filePath,
                                     const QString &newFilePath, const QString &extension, const int &idFormat,
                                     const bool &isAvailable, const int &index)
{
    qDebug() << "channelUnavailable: " << msg;
}

void VideoPlayer::channelChainDownloaded(const QString &fileName, const QString &filePath, const QString &newFilePath,
                                         const QString &extension, const int &idFormat, const bool &isAvailable,
                                         const int &index)
{
    auto chain = m_channelPlaylist.at(index);
    chain.isDownloaded = true;
    m_channelPlaylist[index] = chain;

//    play();

    //  uncomment next 2 lines to unite all in 1 file
//    auto res = std::find_if(m_channelPlaylist.cbegin(), m_channelPlaylist.cend(), [](ChannelSegment cs) {return !cs.isDownloaded;});
//    if (res == m_channelPlaylist.cend())
        emit playUrl(m_channelPlaylist.at(index).localUrl);
    downloadChannelChain(false);
}

void VideoPlayer::downLoadChannelM3UFile(const QUrl &channelUrl)
{        
    QString newFilePath {m_sets->appPath() + "/temp/current_channel/" + channelUrl.fileName()};
    QString filePath = channelUrl.toString();    
    m_fileLoader.doDownload(newFilePath, 0, filePath, "", true, 0, false);
}

void VideoPlayer::parseLine(QString &line)
{    
    if (line.toLower().trimmed().startsWith(m_format.begin_channelInfo.toLower().trimmed(), Qt::CaseInsensitive)) {
        line.remove(m_format.begin_channelInfo);
        line.remove(",");
        double duration = line.toDouble();
        m_channelFile.duration = duration;
    }
    if (line.toLower().trimmed().startsWith(m_format.channel_url.toLower().trimmed(), Qt::CaseInsensitive)) {
        m_channelFile.url = line;
        m_channelFile.localUrl = m_sets->appPath() + "/temp/current_channel/" + QUrl(line).fileName();
//        QFileInfo fInf(line);
//        QString fileExt = fInf.suffix();
//        m_channelFile.localUrl = m_sets->appPath() + "/temp/current_channel/" + m_curSegmentFileName + "." + fileExt;
//        m_channelPlaylist.push(m_channelFile);
        m_channelPlaylist.push_back(m_channelFile);
    }
}

void VideoPlayer::play()
{
    qDebug() << "VideoPlayer::play()";
//    if (m_channelPlaylist.empty()) {
//        if (m_timer.isActive())
//            m_timer.stop();
//    } else {
//        double duration = m_channelPlaylist.front().duration;
//        duration = duration * 1000;
//        m_timer.setInterval(duration);
//        m_timer.start();
//        emit playUrl(m_channelPlaylist.front().url);
//        m_channelPlaylist.pop();
//    }
    if (m_channelPlaylist.isEmpty()) {
        m_timer.stop();
    } else {
        for (int i = 0; i < m_channelPlaylist.size() - 1; ++i) {
            if (m_channelPlaylist.at(i).isPlaying && !m_timer.isActive()) {
                qDebug() << "delete: " << i << " ch: " << m_channelPlaylist.at(i).url;
                QFile file(m_channelPlaylist.at(i).localUrl);
                if (file.exists())
                    file.remove();
                m_channelPlaylist.pop_front();
            }
            if (m_channelPlaylist.at(i).isDownloaded && m_channelPlaylist.at(i).isPlaying && m_timer.isActive()) {
                break;
            }
            if (m_channelPlaylist.at(i).isDownloaded && !m_channelPlaylist.at(i).isPlaying) {
                double duration = m_channelPlaylist.at(i).duration;
                duration = duration * 1000;
                m_timer.setInterval(duration);
                m_timer.start();

                emit playUrl(m_channelPlaylist.at(i).localUrl);

                auto chain = m_channelPlaylist.at(i);
                chain.isPlaying = true;
                m_channelPlaylist[i] = chain;
                break;
            }
        }
//        }
    }
}

void VideoPlayer::channelPlayed(const QString &path)
{
    qDebug() << "VideoPlayer::channelPlayed: " << path;
    QFile file(path);
    if (file.exists())
        file.remove();

    m_channelPlaylist.erase(std::remove_if(m_channelPlaylist.begin(),
                                           m_channelPlaylist.end(),
                                           [path](ChannelSegment cs) {
                                                return cs.localUrl.toLower().trimmed() == path.toLower().trimmed();}),
                            m_channelPlaylist.cend());

    if (m_channelPlaylist.size() == 2)
        playChannel(m_curChannelUrl, false);
    if (m_channelPlaylist.size() == 0)
        emit stopPlaying();
}

void VideoPlayer::appendChannel()
{
    downloadChannelChain(true);
}

void VideoPlayer::downloadChannelChain(const bool &append)
{
    // looking for the first not downloaded channel link, if found then looking for it downloaded version,
    // if found erasing it not downloaded version otherwise download it
    int index {};
    for (const auto &chainLink : m_channelPlaylist) {
        if (!chainLink.isDownloaded) {
            QString curUrlStr = chainLink.url;
            auto res = std::find_if(m_channelPlaylist.cbegin(),
                                    m_channelPlaylist.cend(),
                                    [curUrlStr](ChannelSegment cs) {
                                        return (cs.url.toLower().trimmed() == curUrlStr.toLower().trimmed()
                                                && cs.isDownloaded);
                                    });
            if (res == m_channelPlaylist.end()) {
                QString newFilePath = chainLink.localUrl;
                m_chainLoader.doDownload(newFilePath, index, curUrlStr, "", true, 0, append);
                break;
            } else {
                m_channelPlaylist.erase(std::remove_if(m_channelPlaylist.begin(),
                                                       m_channelPlaylist.end(),
                                                       [curUrlStr](ChannelSegment cs) {
                                                           return (cs.url.toLower().trimmed() == curUrlStr.toLower().trimmed()
                                                                   && !cs.isDownloaded);
                                                       }),
                                        m_channelPlaylist.cend());
            }
        }
        ++index;
    }
}

QString VideoPlayer::getSegmentName()
{
    int num {0};
    int numCur {1};
    if (m_channelFileName.at(0).second) {
        num = 1;
        numCur = 0;
    }

    QPair< QString, bool > cfn = m_channelFileName.at(num);
    cfn.second = true;
    m_channelFileName[num] = cfn;

    cfn = m_channelFileName.at(numCur);
    cfn.second = false;
    m_channelFileName[numCur] = cfn;
    return m_channelFileName.at(num).first;;
}
