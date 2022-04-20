#ifndef PLAYLISTPARSETHREAD_H
#define PLAYLISTPARSETHREAD_H

#include <QThread>
#include <QDebug>
#include <QString>
#include <QMutex>

#include <queue>

#include "./Classes/additional.h"
#include "./Classes/channel.h"
#include "./Gateways/categorygw.h"

class PlaylistParseThread : public QThread
{
    Q_OBJECT
public:
    PlaylistParseThread(QObject *parent = nullptr);
//    virtual ~PlaylistParseThread() = default;

    void run() override;

    void prepareParsing(const QString &filePath, const int &idPlaylist, const int &idFile);

    bool isRunning() const;

signals:
    void channelParsed(Channel &channel, const int &idPlaylist, const int &idFile, const QString &naim);
    void findCategoryId(QString &groupName, const int &idPlaylist, const int &idFile);
    void fullyParsed(const int &idPlaylist, const int &idFile);
//    void finished();

public slots:
    void pushOutCurChannel();
    void setPauseParsing(const bool &isPausing);
    void deleteFrontChannel();

private:
    QString m_filePath {};
    int m_idPlaylist {};
    int m_idFile {};
    bool m_isRunning {false};
    bool m_isPausing {false};
    int idChannel {-1};

    M3UFormat m_format;
    Channel m_channel;
    Channel m_outChannel;
    CategoryGW m_catGW;
    QMutex m_mutex;
    std::queue< Channel > m_channels;

    void parseFile();
    void parseLine(QString &line);
    void parseChannelName(QString &line);
    void checkForSpecSymbols(QString &line);
    QString getSpecValue(QString &line, const QString &paramName);
    void removeLineBegin(QString &line);

};

#endif // PLAYLISTPARSETHREAD_H
