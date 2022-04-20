#ifndef PLFILES_H
#define PLFILES_H

#include "./Classes/playlist.h"
#include "./Gateways/plfilegw.h"
#include "./Classes/settings.h"
#include "./Thread/filecopythread.h"
#include "./Classes/filedownloader.h"

#include "./Classes/additional.h"


#include <QObject>
#include <QVector>
#include <QModelIndex>
#include <QUrl>

#include <QEventLoop>

class PlFiles : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int rowCount READ rowCount NOTIFY rowCountChanged)
    Q_PROPERTY(int idPlaylist READ idPlaylist WRITE setIdPlaylist)
    Q_PROPERTY(int idFile READ curIdFile)
    Q_PROPERTY(QString fileName READ fileName)
    Q_PROPERTY(QUrl curDir READ getCurDir WRITE setCurDir)

public:    
    PlFiles(QObject *parent = nullptr);
    PlFiles(Settings &sets, QObject *parent);
//    PlFiles* operator=(const PlFiles* orig);
    virtual ~PlFiles();

    QVector<PlFile> items() const;
    bool setItemAt(int index, PlFile &item);
    void addItem(const int &idFile, const int &idPlaylist, const QString &fileName, const QString &filePath,
                 const QString &filePathLocal, const int &idFormat, const bool &isAvailable, const QString &format);

    int rowCount() const;
    QString fileName() const;

    int curIndex() const;

    int idPlaylist() const;
    void setIdPlaylist(const int &idPlaylist);

    QUrl getCurDir();
    void setCurDir(const QUrl &curDir);

    void setSets(const Settings &value);
    int curIdFile() const;
    void initConnections();


signals:
    void beforeItemAppended();
    void afterItemAppended();

    void beforeItemRemoved(int index);
    void afterItemRemoved();

    void selectItem(int index);
    void itemChanged(int index);
    void rowCountChanged(int rows);

    //signals for other classes who needs changes from current, f.e. DB
    void itemEdited(const int &index, const int &idFile, const int &idPlaylist, const QString &fileName, const QString &filePath,
                    const QString &filePathLocal, const int &idFormat, const bool &isAvailable);
    void itemAppended(const int &index, const int &idPlaylist, const QString &fileName, const QString &filePath,
                      const QString &filePathLocal, const int &idFormat, const bool &isAvailable);
    void itemDeleted(const int &index, const int &idFile);
    void checkFileExtension(const int &index, QString &fullFilePath, QString &newFilePath, const QString &extension, const uint &sender);
    void extensionChecked(const int &index, QString &fullFilePath, QString &newFilePath, const QString &extension,
                          const bool &isValid, const int &idFormat);
    void errorEmited(QString errorMsg);
    void listChanged();
    void itemFullyAdded();

    void fileFullyAdded(const QString &newFilePath);
    void filesScrolled(const int &idPlaylist, const int &idFile);


public slots:
    void open(const int &idPlaylist = 0);
    void open(const bool &allFilesInPlaylist, const int &idFile);
    void appendNewItem();
    void addItemFromLocalFile(int index, QUrl filePath);
    void addItemFromBuffer(int index, QUrl filePath);
    void addItemFromUrl(int index, QUrl fileUrl);
    void refresh(int index);
    void refreshLocalFile(int index, QString filePath);
    void refreshUrlFile(int index, QString filePath);
    void removeCurrentItem(int index);
    QString getClipboardString();
    void setFilePath(int index, QString filePath);
    void selectFile();


    void downloadFileFromInternet(const int &index, QString &fullFilePath, QString &newFilePath, const QString &extension,
                                  const bool &isValid, const int &idFormat);

    void setNewItemIdFile(const int &index, const int &idFile);
    void addCopiedFileInfo(const QString &fileName, const QString &filePath, const QString &newFilePath,
                    const QString &extension, const int &idFormat, const bool &isAvailable, const int &index);
    void fileNotCopied(const QString &errorMsg, const QString &fileName, const QString &filePath, const QString &newFilePath,
                       const QString &extension, const int &idFormat, const bool &isAvailable, const int &index);

    void extensionChecked(const int &index, QString &fullFilePath, QString &newFilePath, const QString &extension,
                          const bool &isValid, const int &idFormat, const uint &sender);    //sender = 0 - emited from adding local file, 1 - from url file
    void clear();
    void scroll(int index);

private:
    QVector<PlFile> m_files;
    std::shared_ptr< Settings > m_sets;
    FileCopyThread m_plThr {this};
    FileDownloader m_fileLoader;


    QString m_fileName;
    QStringList m_filesName;
    int m_idPlaylist;
    QString m_curDir;
    int m_curIdFile;

    QEventLoop m_eventLoop;

    void waitForId();

};

#endif // PLFILES_H
