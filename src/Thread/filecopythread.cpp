#include "filecopythread.h"

FileCopyThread::FileCopyThread(QObject *parent) : QThread(parent)
{
    qDebug() << "fileworkerthread() " << QThread::currentThreadId() << " parent " << parent;
}

void FileCopyThread::copyFile(QString &oldPath, QString &newPath)
{
    m_msg = "";
    m_file.setFileName(newPath);
    if (m_file.exists()) {
        if (!m_file.remove()) {
            m_msg = "Error! Cannot delete existing old playlist file from local directory.";
        }
    }

    m_file.setFileName(oldPath);
    if (m_msg == "") {
        if (m_file.copy(newPath)) {
            qDebug() << "thread: file copied";
        } else {
            m_msg = "Error! Cannot copy new playlist file to local directory.";
        }
    }

    if (m_msg != "")
        emit error(m_msg, m_fName, m_fullFilePath, m_newFilePath, m_extension, m_idFormat, m_isValid, m_index);



}

void FileCopyThread::extensionChecked(const int &index, QString &fullFilePath, QString &newFilePath, const QString &extension,
                                      const bool &isValid, const int &idFormat)
{
    qDebug() << "filecopythread: ext checked()";
    m_isValid = isValid;
    QFileInfo fInf(fullFilePath);
    m_fName = fInf.fileName();
    m_extension = extension;
    m_idFormat = idFormat;
    m_fullFilePath = fullFilePath;
    m_newFilePath = newFilePath;
    m_index = index;
    if (isValid) {                
        m_isAvailable = true;
    } else {
        qDebug() << "filecopythread: ext is " << isValid << " fullFilePath: " << fullFilePath;
        m_msg = tr("File format is invalid");
        m_isAvailable = false;
        emit error(m_msg, m_fName, fullFilePath, newFilePath, extension, idFormat, isValid, index);

        m_fullFilePath = "";
        m_newFilePath = "";
        m_fName = "";
        m_extension = "";
        m_idFormat = 0;

        m_index = -1;


    }
}

void FileCopyThread::run()
{
    qDebug() << "FileWorkerThread is running " << QThread::currentThreadId()  << " parent " << QThread::parent();
    if (m_isValid) {
        QDir newDir;
        QFileInfo fInf(m_newFilePath);
        if (newDir.mkpath(fInf.absolutePath())) {
            copyFile(m_fullFilePath, m_newFilePath);

            emit fileCopied(m_fName, m_fullFilePath, m_newFilePath, m_extension, m_idFormat, m_isAvailable, m_index);
        }
    }

}

void FileCopyThread::copyFile(const int &index, const QUrl &filePath, QString &newFilePath)
{    
    if (filePath.isLocalFile()) {
        QString fullPath = filePath.toLocalFile();
        QFileInfo fInf(fullPath);
        QString fileExt = fInf.suffix();        
        uint sender {0};
        m_newFilePath = newFilePath;

        emit checkFileExtension(index, fullPath, newFilePath, fileExt, sender);
    }
}

void FileCopyThread::refreshFile(const int &index, const QUrl &filePath, QString &newFilePath)
{
    m_newFilePath = newFilePath;
    m_fullFilePath = filePath.path();
    m_index = index;
    m_isValid = true;
    m_isAvailable = true;
    uint sender {0};
    QFileInfo fInf(m_fullFilePath);
    QString fileExt = fInf.suffix();
    emit checkFileExtension(index, m_fullFilePath, newFilePath, fileExt, sender);

}
