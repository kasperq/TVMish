#ifndef CHANNELS_H
#define CHANNELS_H

#include <QObject>
#include <QVector>
#include <QMutex>
#include <QQueue>
#include <queue>

#include "./Classes/additional.h"
#include "./Classes/channel.h"
#include "./Thread/playlistparsethread.h"


class Channels : public QObject
{
    Q_OBJECT
public:
    explicit Channels(QObject *parent = nullptr);
    virtual ~Channels();

    QVector< Channel > items() const;
    bool setItemAt(int index, Channel &item);
    void addItem(const int &idChannel, const QString &naim, const QString &url, const int &idFile, const int &idPlaylist,
                 const int &idCategory, const int &numFile, const int &numPlaylist, const int &archDays, const int &idLogo);
    void clear();

    int rowCount() const;

    int getIdPlaylist() const;
    int getIdFile() const;
    void setIdFile(const int &newCurIdFile);
    void setIdPlaylist(const int &newCurIdPlaylist);

signals:
    void itemEdited(const int &index, const int &idChannel, const QString &naim, const QString &url, const int &idFile,
                    const int &idPlaylist, const int &idCategory, const int &numFile, const int &numPlaylist,
                    const int &archDays, const int &idLogo);
    void itemAppended(const int &index, const QString &naim, const QString &url, const int &idFile, const int &idPlaylist,
                      const int &idCategory, const int &numFile, const int &numPlaylist, const int &archDays,
                      const int &idLogo, const bool &generateNums);
    void channelParsed(/*const int &idPlaylist, const int &idFile*/);
    void getNums(const int &index, const int &idPlaylist, const int &idFile);
    void channelDeleted(const int &idChannel);

    void beforeItemAppended();
    void afterItemAppended();

    void beforeItemRemoved(int index);
    void afterItemRemoved();

    void selectItem(int index);
    void itemChanged(int index);
    void listChanged();

    void pauseParsing(const bool &isPausing);
    void saveChannels();

    void prepareForAdding();
    void addForInsert(const int &index, const QString &naim, const QString &url, const int &idFile, const int &idPlaylist,
                      const int &idCategory, const int &numFile, const int &numPlaylist, const int &archDays,
                      const int &idLogo, const bool &generateNums);

public slots:
    void open(const int &idPlaylist, const int &idFile);
//    void parseFile(const QString &filePath, const int &idPlaylist, const int &idFile);
    void update(const QString &filePath);
    void addParsedChannel(Channel &newChannel, const int &idPlaylist, const int &idFile);
    void addChannel(/*const int &idPlaylist, const int &idFile*/);
    void setNewIdChannelAndNums(const int &index, const int &newId, const int &numFile, const int &numPlaylist,
                                const int &idFile, const int &idPlaylist);
    void deleteChannel(const int &index);
    void save();
//    void proccessParsedChannel();


private:
    QVector< Channel > m_channels;
//    QQueue< Channel > m_channelsToAdd;
    std::queue< Channel > m_channelsToAdd;
    std::queue< Channel > m_channelsBuffer;
    M3UFormat m_format;
    Channel channel;
    PlaylistParseThread m_parseThrd;
    QMutex m_mutex;

    int m_curIdFile {};
    int m_curIdPlaylist {};
    bool m_isInsertingChannel {false};
    bool m_isPausedParsing {false};
    uint m_parseLimit {500};

    void initConnections();
    void checkQueuesAndParse();
//    void parseLine(QString &line);

};

#endif // CHANNELS_H
