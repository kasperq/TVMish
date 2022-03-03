#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QtNetwork>

class QSslError;

class FileDownloader : public QObject
{
    Q_OBJECT
public:
    FileDownloader();
    virtual ~FileDownloader() = default;

    void doDownload(QString &newPath, const int &index, QString &fullFilePath, const QString &extension,
                    const bool &isValid, const int &idFormat);
    bool saveFileName();
    bool saveToDisk(const QString &filename, QIODevice *data);
    static bool isHttpRedirect(QNetworkReply *reply);

signals:
    void error(const QString &msg, const QString &fileName, const QString &filePath, const QString &newFilePath,
               const QString &extension, const int &idFormat, const bool &isAvailable, const int &inde);
    void fileDownloaded(const QString &fileName, const QString &filePath, const QString &newFilePath,
                        const QString &extension, const int &idFormat, const bool &isAvailable, const int &index);

public slots:
    void downloadFinished(QNetworkReply *reply);
    void sslErrors(const QList<QSslError> &errors);

private:
    QNetworkAccessManager manager;
    QList<QNetworkReply *> currentDownloads;
    QFile m_file;

    QString m_newPath;
    QString m_fullFilePath;
    QString m_newFilePath;
    QString m_fName;
    QString m_extension;
    int m_idFormat;
    bool m_isAvailable {false};
    bool m_isValid {false};
    int m_index {-1};

    QString m_errorMsg {tr("")};

};

#endif // FILEDOWNLOADER_H
