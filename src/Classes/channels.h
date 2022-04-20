#ifndef CHANNELS_H
#define CHANNELS_H

#include <QObject>
#include <QVector>
#include <QMutex>
#include <QQueue>
#include <QTimer>

#include <queue>

#include "./Classes/additional.h"
#include "./Classes/channel.h"
#include "./Thread/playlistparsethread.h"
#include "./Thread/filecopythread.h"
#include "./Classes/settings.h"


class Channels : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int rowCount READ rowCount NOTIFY rowCountChanged)
    Q_PROPERTY(bool isAllFilesSelected READ isAllFilesSelected /*WRITE setIsAllFilesSelected*/ NOTIFY isAllFilesSelectedChanged)
public:
    explicit Channels(QObject *parent = nullptr);
    virtual ~Channels();

    QVector< Channel > items() const;
    bool setItemAt(int index, Channel &item);
    void addItem(const int &idChannel, const QString &naim, const QString &url, const int &idFile, const int &idPlaylist,
                 const int &idCategory, const int &numPlaylist, const int &archDays, const int &idLogo, const QString &catNaim,
                 const QUrl &logoPath, const bool &isFavorite, const int &numFav, const bool &isAvailable);
    void clear();

    int rowCount() const;

    int getIdPlaylist() const;
    int getIdFile() const;
    void setIdFile(const int &newCurIdFile);
    void setIdPlaylist(const int &newCurIdPlaylist);
    bool isAllFilesSelected() const;
    void setSets(const Settings &value);
    void initConnections();

signals:
    // for external use in GW
    void itemEdited(const int &index, const int &idChannel, const QString &naim, const QString &url, const int &idFile,
                    const int &idPlaylist, const int &idCategory, const int &numPlaylist, const int &archDays,
                    const int &idLogo, const bool &isFavorite, const int &numFav, const bool &isAvailable);
    void itemAppended(const int &index, const QString &naim, const QString &url, const int &idFile, const int &idPlaylist,
                      const int &idCategory, const int &numPlaylist, const int &archDays, const int &idLogo,
                      const bool &generateNums, const bool &isFavorite, const int &numFav, const bool &isAvailable);
    void channelParsed(/*const int &idPlaylist, const int &idFile*/);
    void channelAdded();
    void getNums(const int &index, const int &idPlaylist, const int &idFile);
    void channelDeleted(const int &index, const int &idChannel);
    void saveChannels();
    void setFavorite(const int &index, const bool &isFav, const int &idChannel, const int &idPlaylist);
    void openChannels(const int &idPlaylist, const int &idFile, const QString &filterNaim, const int &idCategory);

    void insertLogo(const int &index, const QString &channelName, const QString &logoPath, const QString &logoPathSmall,
                    const QString &epgCode);
    void changeLogo(const int &index, const int &idLogo, const QString &channelName, const QString &logoPath,
                    const QString &logoPathSmall, const QString &epgCode);
    void changeLogoPath(const int &idLogo, const QString &logoPath, const QString &logoPathSmall);


    void channelsScrolled(const int &idChannel);

    // for external use in QML
    void rowCountChanged(int rows);

    void beforeItemAppended();
    void afterItemAppended();

    void beforeItemRemoved(int index);
    void afterItemRemoved();

    void selectItem(int index);
    void itemChanged(int index);
    void logoChanged(int indexChange, QUrl logoPath);
    void listChanged();
    void isAllFilesSelectedChanged(const bool &allSelected);
    void channelsAdded(const bool &result);

    // for internal use
    void pauseParsing(const bool &isPausing);


public slots:
    void open(const int &idPlaylist, const int &idFile);    
    void scroll(int index);

    void update(const QString &filePath);
    void addParsedChannel(Channel &newChannel, const int &idPlaylist, const int &idFile);    
    void setNewIdChannelAndNums(const int &index, const int &newId, const int &numPlaylist, const int &idFile, const int &idPlaylist);
    void restartInserting();

    void deleteChannel(const int &index);
    void deleteAllInFile(const int &idFile);

    void move(int index, QString type = "down");
    void setIsFavorite(int index, bool isFav);   
    void setFavoriteNum(const int &index, const int &idChannel, const int &num);
    void filter(QString text);
    void filterCategory(const int &idCategory);
    void find(QString text);
    void setLogo(int index, QUrl filePath);
    void logoCopied(const QString &fileName, const QString &filePath, const QString &newFilePath,
                    const QString &extension, const int &idFormat, const bool &isAvailable,
                    const int &index);
    void logoInserted(const int &index, const int &newId, const QString &logoPath);

    void save(const int &idPlaylist, const int &idFile);


private:
    QVector< Channel > m_channels;
    std::shared_ptr< Settings > m_sets;

    Channel m_newChannel;
    int m_curAttempt {};
    const int m_maxAttempts {5};
    QTimer m_timer;
    M3UFormat m_format;
    Channel channel;
    PlaylistParseThread m_parseThrd;
    QMutex m_mutex;
    FileCopyThread m_plThr {this};

    int m_curIdChannel {};
    int m_curIdFile {};
    int m_curIdPlaylist {};
    bool m_isInsertingChannel {false};
    bool m_isPausedParsing {false};
    uint m_parseLimit {50};
    QString m_filterNaim;
    bool m_isSaving {false};
    int m_curIdCategory {};
};

#endif // CHANNELS_H
