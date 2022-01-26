#ifndef PLAYLISTTHREAD_H
#define PLAYLISTTHREAD_H

#include <QThread>
#include <QObject>
#include <QFile>
#include <QDebug>
#include <QUrl>
#include <QFileInfo>
#include <QDir>

#include "Classes/plfile.h"

class FileWorkerThread : public QThread
{
    Q_OBJECT
public:
//    FileWorkerThread();
    FileWorkerThread(QObject *parent = nullptr);

    void run() override;

    void copyFile(const int &index, const QUrl &filePath);
    void copyFile(QString &oldPath, QString &newPath);

signals:
    void error(const QString &msg);
    void copyFinished();
    void checkFileExtension(const int &index, QString &fullFilePath, const QString &extension);
    void fileCopied(const QString &fileName, const QString &filePath, const QString &newFilePath,
                    const QString &extension, const int &idFormat, const bool &isAvailable,
                    const int &index);

public slots:
    void extensionChecked(const int &index, QString &fullFilePath,
                                    const QString &extension, const bool &isValid,
                                    const int &idFormat, const QString &appPath);

private:
    QFile m_file;
    QString msg {tr("")};

    int type {1};
};

#endif // PLAYLISTTHREAD_H
