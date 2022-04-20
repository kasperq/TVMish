#include "filedownloader.h"

#include <QSslError>
#include <QDataStream>
FileDownloader::FileDownloader()
{
    connect(&manager, &QNetworkAccessManager::finished, this, &FileDownloader::downloadFinished);
}

void FileDownloader::doDownload(QString &newPath, const int &index, QString &fullFilePath, const QString &extension,
                                const bool &isValid, const int &idFormat, const bool &isAppend)
{
    if (isValid) {
//        qDebug() << "new: " << newPath << " file: " << fullFilePath;
        m_newPath = newPath;
        QFileInfo fInf(fullFilePath);
        m_fName = fInf.fileName();
        m_isAvailable = true;
        QUrl url(fullFilePath);

        m_index = index;
        m_extension = extension;
        m_idFormat = idFormat;
        m_fullFilePath = fullFilePath;
        m_isAppend = isAppend;

        QNetworkRequest request(url);
        QNetworkReply *reply = manager.get(request);

#if QT_CONFIG(ssl)
        connect(reply, &QNetworkReply::sslErrors, this, &FileDownloader::sslErrors);
#endif
        currentDownloads.append(reply);
    }
}

bool FileDownloader::saveFileName()
{    
    m_file.setFileName(m_newPath);
    if (!m_isAppend && m_file.exists()) {
        if (!m_file.remove()) {
            m_errorMsg = "Error! Cannot delete existing old playlist file from local directory.";
            m_isAvailable = false;
            emit error(m_errorMsg, m_fName, m_fullFilePath, m_newPath, m_extension, m_idFormat, m_isAvailable, m_index);
            return false;
        }
    }
    m_file.close();
    QDir newDir;
    QFileInfo fInf(m_newPath);
    if (newDir.mkpath(fInf.absolutePath()))
        return true;
    return true;
}

bool FileDownloader::saveToDisk(const QString &filename, QIODevice *data)
{
    QFile file(filename);
    m_errorMsg.clear();
    if (m_isAppend) {
        if (!file.open(QIODevice::Append))
            m_errorMsg = "Could not open " + filename + " for writing: " + file.errorString();
    } else {
        if (!file.open(QIODevice::WriteOnly))
            m_errorMsg = "Could not open " + filename + " for writing: " + file.errorString();
    }
    if (m_errorMsg.isEmpty()) {
        if (file.isOpen())
            file.write(data->readAll());
        file.close();
        qDebug() << "Download of " << m_fullFilePath << " succeeded (saved to " << m_newPath << ")";
        emit fileDownloaded(m_fName, m_fullFilePath, m_newPath, m_extension, m_idFormat, m_isAvailable, m_index);

        return true;
    } else {
        qDebug() << "Downloading error: " << m_errorMsg;
        m_isAvailable = false;
        emit error(m_errorMsg, m_fName, m_fullFilePath, m_newPath, m_extension, m_idFormat, m_isAvailable, m_index);

        return false;
    }
}

bool FileDownloader::isHttpRedirect(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    return statusCode == 301 || statusCode == 302 || statusCode == 303
            || statusCode == 305 || statusCode == 307 || statusCode == 308;
}

void FileDownloader::downloadFinished(QNetworkReply *reply)
{    
    QUrl url = reply->url();
    if (reply->error()) {
        m_errorMsg = "Download of " + QString::fromLatin1(url.toEncoded().constData()) + " failed: " + reply->errorString();
        qDebug() << m_errorMsg;
        m_isAvailable = false;
        emit error(m_errorMsg, m_fName, m_fullFilePath, m_newPath, m_extension, m_idFormat, m_isAvailable, m_index);
    } else {        
        if (isHttpRedirect(reply)) {
            m_errorMsg = tr("Request was redirected.");
            qDebug() << m_errorMsg;
            m_isAvailable = false;
            emit error(m_errorMsg, m_fName, m_fullFilePath, m_newPath, m_extension, m_idFormat, m_isAvailable, m_index);
        } else {            
            if (saveFileName()) {
                if (saveToDisk(m_newPath, reply)) {
//                    qDebug() << "Download of " << url.toEncoded().constData() << " succeeded (saved to " << m_newPath << ")";
                }
            }
        }
    }

    currentDownloads.removeAll(reply);
    reply->deleteLater();
}

void FileDownloader::sslErrors(const QList<QSslError> &errors)
{
#if QT_CONFIG(ssl)
    for (const QSslError &error : errors)
        qDebug() << "SSL error: " << error.errorString();
#else
    Q_UNUSED(sslErrors);
#endif
}
