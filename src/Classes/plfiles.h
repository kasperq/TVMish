#ifndef PLFILES_H
#define PLFILES_H

#include "./Classes/playlist.h"
#include "./Gateways/plfilegw.h"
#include "./Classes/settings.h"
#include "./Thread/fileworkerthread.h"

#include <QObject>
#include <QVector>
#include <QModelIndex>
#include <QUrl>

class PlFiles : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int rowCount READ rowCount NOTIFY rowCountChanged)
    Q_PROPERTY(int idPlaylist READ idPlaylist WRITE setIdPlaylist)
    Q_PROPERTY(QString fileName READ fileName)
    Q_PROPERTY(QUrl curDir READ getCurDir WRITE setCurDir)

public:    
    PlFiles(QObject *parent = nullptr);
    PlFiles(Settings &sets);
//    PlFiles* operator=(const PlFiles* orig);
    virtual ~PlFiles();

    QVector<PlFile> items() const;
    bool setItemAt(int index, PlFile &item);
    void addItem(const int &idFile, const int &idPlaylist, const QString &fileName, const QString &filePath,
                 const QString &filePathLocal, const int &idFormat, const bool &isAvailable, const QString &format);
    void clear();
    int rowCount() const;
    QString fileName() const;

    int curIndex() const;

    int idPlaylist() const;
    void setIdPlaylist(int idPlaylist);

    QUrl getCurDir();
    void setCurDir(const QUrl &curDir);

    void setSets(const Settings &value);


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
    void checkFileExtension(const int &index, QString &fullFilePath, const QString &extension);
    void extensionChecked(const int &index, QString &fullFilePath,
                                    const QString &extension, const bool &isValid,
                                    const int &idFormat, const QString &appPath);
    void errorEmited(QString errorMsg);



public slots:
    void open(int idPlaylist);
    void appendNewItem();
    void addItemFromLocalFile(int index, QUrl filePath);
    void addItemFromUrl(int index, QUrl fileUrl);
    void removeCurrentItem(int index);
    QString getClipboardString();
    void setFilePath(int index, QString filePath);
    void selectFile();
    void copyFinished();

    void setNewItemIdFile(const int &index, const int &idFile);
    void addCopiedFileInfo(const QString &fileName, const QString &filePath, const QString &newFilePath,
                    const QString &extension, const int &idFormat, const bool &isAvailable,
                    const int &index);
    void fileNotCopied(const QString &errorMsg);

    void extensionChecked(const int &index, QString &fullFilePath, const QString &extension,
                          const bool &isValid, const int &idFormat);

private:
    QVector<PlFile> m_files;
    std::shared_ptr< Settings > m_sets;
    FileWorkerThread m_plThr;

    QString m_fileName;
    QStringList m_filesName;
    int m_idPlaylist;
    QString m_curDir;

    bool isFormatValid(QString &format);
    void copyLocalFile(QString &oldPath, QString &newPath);
    void initConnections();


};

#endif // PLFILES_H
