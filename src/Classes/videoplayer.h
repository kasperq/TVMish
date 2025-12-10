#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QObject>
#include <QUrl>
#include <QString>
#include <QTimer>

// #include <queue>

#include "Classes/filedownloader.h"
#include "Classes/settings.h"
#include "Classes/additional.h"


class VideoPlayer : public QObject
{
    Q_OBJECT
public:
    explicit VideoPlayer(QObject *parent = nullptr);
    virtual ~VideoPlayer();

    void initConnections();
    void setSets(const Settings &value);

signals:
    void playUrlInMiniPlayer(QUrl channelUrl);
    void playUrlInPlayer(QUrl channelUrl);
    void stopPlayingMiniPlayer();
    void stopPlayingPlayer();

public slots:
    // starts playing new channel by channel URL
    void playChannel(QUrl channelUrl, const bool &fromBeginning);
    // parse m3u file for URL to download stream video files
    void parseChannelPlaylist(const QString &fileName, const QString &filePath, const QString &newFilePath,
                        const QString &extension, const int &idFormat, const bool &isAvailable, const int &index);
    void channelUnavailable(const QString &msg, const QString &fileName, const QString &filePath, const QString &newFilePath,
                            const QString &extension, const int &idFormat, const bool &isAvailable, const int &index);
    // starts playing downloaded stream video
    void channelChainDownloaded(const QString &fileName, const QString &filePath, const QString &newFilePath,
                                const QString &extension, const int &idFormat, const bool &isAvailable, const int &index);
    void play();
    void channelPlayed(const QString &path);
    void appendChannel();

    void pauseDownloadLinks(const bool &isPause);
    void setFullScreen(const bool &isFull);    

private:    
    QList< QPair< QString, bool> > m_channelFileName {{"channel1", false}, {"channel2", false}};
    QString m_curSegmentFileName {""};
//    QList< QString > m_channelFileName {"channel1", "channel2"};
    QUrl m_curChannelUrl;
    FileDownloader m_fileLoader;
    std::shared_ptr< Settings > m_sets;
//    std::queue< ChannelSegment > m_channelPlaylist;
    QList< ChannelSegment > m_channelPlaylist;

    M3UFormat m_format;
    ChannelSegment m_channelFile;
    QTimer m_timer;
    FileDownloader m_chainLoader;

    bool m_isFullScreen {false};
    bool m_isPauseDownload {false};

    //emit download m3u channel playlist file
    void downLoadChannelM3UFile(const QUrl &channelUrl);
    // parse single line in a correct M3U file and push URL to URL's list
    void parseLine(QString &line);

    void downloadChannelChain(const bool &append);
    QString getSegmentName();
};

#endif // VIDEOPLAYER_H
