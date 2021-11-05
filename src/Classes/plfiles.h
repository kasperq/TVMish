#ifndef PLFILES_H
#define PLFILES_H

#include "./Classes/playlist.h"
#include "./Gateways/plfilegw.h"
#include "./Classes/settings.h"
#include "./Thread/playlistthread.h"

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
    PlFiles(PlFileGW *plGW);
    PlFiles(QObject *parent = nullptr);
    PlFiles* operator=(const PlFiles* orig);
    virtual ~PlFiles();

    QVector<PlFile> items() const;
    bool setItemAt(int index, PlFile &item);
    void addItem(int *idFile, int *idPlaylist, QString *fileName, QString *filePath, QString *filePathLocal,
                 int *idFormat, bool *isAvailable, QString *format);

    int rowCount() const;
    QString fileName() const;

    int curIndex() const;

    int idPlaylist() const;
    void setIdPlaylist(int idPlaylist);

    QUrl getCurDir();
    void setCurDir(const QUrl &curDir);


signals:
    void beforeItemAppended();
    void afterItemAppended();

    void beforeItemRemoved(int index);
    void afterItemRemoved();

    void selectItem(int index);
    void itemChanged(int index);
    void rowCountChanged(int rows);


public slots:
    void open(int idPlaylist);
    void addItem();
    void addItemFromLocalFile(int index, QUrl filePath);
    void addItemFromUrl(int index, QUrl fileUrl);
    void removeCurrentItem(int index);
    QString getClipboardString();
    void setFilePath(int index, QString filePath);
    void selectFile();
    void copyFinished();


private:
    QVector<PlFile> m_files;
    PlFileGW *m_fileGw;
    std::shared_ptr<Settings> m_sets;
    PlaylistThread m_plThr;

    QString m_fileName;
    QStringList m_filesName;
    int m_idPlaylist;
    QString m_curDir;

    bool isFormatValid(QString *format);
    void copyLocalFile(QString *oldPath, QString *newPath, int *idFile);


};

#endif // PLFILES_H
