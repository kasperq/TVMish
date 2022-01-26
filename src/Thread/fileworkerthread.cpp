#include "fileworkerthread.h"

//FileWorkerThread::FileWorkerThread()
//{
//    qDebug() << "fileworkerthread() " << this->thread();
//}

FileWorkerThread::FileWorkerThread(QObject *parent) : QThread(parent)
{
    qDebug() << "fileworkerthread() " << QThread::currentThreadId() << " parent " << parent;
}

void FileWorkerThread::copyFile(QString &oldPath, QString &newPath)
{
    msg = "";
    m_file.setFileName(newPath);
    if (m_file.exists()) {
        if (!m_file.remove()) {
            msg = "Error! Cannot delete existing old playlist file from local directory.";
        }
    }

    m_file.setFileName(oldPath);
    if (msg == "") {
        if (m_file.copy(newPath)) {
            qDebug() << "thread: file copied";
            emit copyFinished();
        } else {
            msg = "Error! Cannot copy new playlist file to local directory.";
        }
    }

    if (msg != "")
        emit error(msg);



}

void FileWorkerThread::extensionChecked(const int &index, QString &fullFilePath, const QString &extension,
                                                  const bool &isValid, const int &idFormat, const QString &appPath)
{
    if (isValid) {
        QFileInfo fInf(fullFilePath);
        QString fName {fInf.fileName()};
        bool isAvailable {true};        
        QString newPath = appPath + "/temp/";
        QString newFilePath = newPath + fName;
        QDir newDir;
        if (newDir.mkpath(newPath)) {
            copyFile(fullFilePath, newFilePath);

            emit fileCopied(fName, fullFilePath, newFilePath, extension, idFormat, isAvailable, index);
        }
    } else {
        msg = tr("File format is invalid");
        emit error(msg);
    }
}

void FileWorkerThread::run()
{
    qDebug() << "FileWorkerThread is running " << QThread::currentThreadId()  << " parent " << QThread::parent();

}

void FileWorkerThread::copyFile(const int &index, const QUrl &filePath)
{    
    if (filePath.isLocalFile()) {
        QString fullPath = filePath.toLocalFile();
        QFileInfo fInf(fullPath);
        QString fileExt = fInf.completeSuffix();
        emit checkFileExtension(index, fullPath, fileExt);
    }
}
