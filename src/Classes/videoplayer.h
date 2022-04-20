#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QObject>
#include <QUrl>
#include <QString>
#include <QTimer>

#include <queue>

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
    void playUrl(QUrl channelUrl);
    void stopPlaying();

public slots:
    void playChannel(QUrl &channelUrl, const bool &fromBeginning);
    void parseChannelPlaylist(const QString &fileName, const QString &filePath, const QString &newFilePath,
                        const QString &extension, const int &idFormat, const bool &isAvailable, const int &index);
    void channelUnavailable(const QString &msg, const QString &fileName, const QString &filePath, const QString &newFilePath,
                            const QString &extension, const int &idFormat, const bool &isAvailable, const int &index);

    void channelChainDownloaded(const QString &fileName, const QString &filePath, const QString &newFilePath,
                                const QString &extension, const int &idFormat, const bool &isAvailable, const int &index);
    void play();
    void channelPlayed(const QString &path);
    void appendChannel();

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



    void downLoadChannelM3UFile(const QUrl &channelUrl);
    void parseLine(QString &line);

    void downloadChannelChain(const bool &append);
    QString getSegmentName();
};

#endif // VIDEOPLAYER_H
