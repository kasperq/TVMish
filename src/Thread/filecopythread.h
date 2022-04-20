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

class FileCopyThread : public QThread
{
    Q_OBJECT
public:
    FileCopyThread(QObject *parent = nullptr);

    void run() override;

    void copyFile(const int &index, const QUrl &filePath, QString &newFilePath, const bool &checkExtension);
    void refreshFile(const int &index, const QUrl &filePath, QString &newFilePath);


signals:
    void error(const QString &msg, const QString &fileName, const QString &filePath, const QString &newFilePath,
               const QString &extension, const int &idFormat, const bool &isAvailable, const int &index);
    void checkFileExtension(const int &index, QString &fullFilePath, QString &newFilePath, const QString &extension, const uint &sender);
    void fileCopied(const QString &fileName, const QString &filePath, const QString &newFilePath,
                    const QString &extension, const int &idFormat, const bool &isAvailable,
                    const int &index);

public slots:
    void extensionChecked(const int &index, QString &fullFilePath, QString &newFilePath, const QString &extension,
                          const bool &isValid, const int &idFormat);

private:
    QFile m_file;
    QString m_msg {tr("")};
    QString m_fullFilePath;
    QString m_newFilePath;
    QString m_fName;
    QString m_extension;
    int m_idFormat;
    bool m_isAvailable {false};
    bool m_isValid {false};
    int m_index {-1};

    int type {1};

    void copyFile(QString &oldPath, QString &newPath);
};

#endif // PLAYLISTTHREAD_H
