#include "plfiles.h"

#include <QFile>
#include <QUrl>
#include <QFileInfo>
#include <QClipboard>
#include <QApplication>
#include <QFileDialog>

PlFiles::PlFiles(QObject *parent) : QObject(parent)
{
//    qDebug() << "constructor without params PlFiles";
    m_idPlaylist = 1;
    initConnections();

//    m_plThr = new FileWorkerThread(this);
//    m_plThr.start();

//    m_sets = std::make_shared<Settings>();
}

PlFiles::PlFiles(Settings &sets, QObject *parent) : QObject(parent), m_sets(std::make_shared< Settings> (sets))
{
    qDebug() << "constructor without params PlFiles";
    m_idPlaylist = 1;
    initConnections();

//    m_plThr = new FileWorkerThread(this);
//    m_plThr.start();
//    m_sets = std::make_shared<Settings>();
}

//PlFiles *PlFiles::operator=(const PlFiles *orig)
//{
//    m_files.clear();

//    QString fileName;
//    QString filePath;
//    QString filePathLocal;
//    QString format;
//    bool isAvailable;
//    int idPlaylist;
//    int idFile;
//    int idFormat;

//    qDebug() << "===";


//    for(QVector< PlFile >::iterator pI = orig->items().begin(); pI != orig->items().end(); ++ pI) {
//        fileName = pI->fileName();
//        filePath = pI->filePath();
//        filePathLocal = pI->filePathLocal();
//        format = pI->format();
//        isAvailable = pI->isAvailable();
//        idPlaylist = pI->idPlaylist();
//        idFile = pI->idFile();
//        idFormat = pI->idFormat();

//        addItem(idFile, idPlaylist, fileName, filePath, filePathLocal, idFormat, isAvailable, format);
//    }
//    m_idPlaylist = orig->m_idPlaylist;

//    return this;
//}

PlFiles::~PlFiles()
{
    m_plThr.quit();
    m_plThr.wait();
}

QVector<PlFile> PlFiles::items() const
{
    return m_files;
}

bool PlFiles::setItemAt(int index, PlFile &item)
{
    if (index < 0 || index >= m_files.size())
        return false;

    const PlFile &oldItem = m_files.at(index);
    if (item.idPlaylist() == oldItem.idPlaylist()
        && item.idFile() == oldItem.idFile()
        && item.fileName() == oldItem.fileName()
        && item.filePath() == oldItem.filePath()
        && item.filePathLocal() == oldItem.filePathLocal()
        && item.idFormat() == oldItem.idFormat()
        && item.format() == oldItem.format()
        && item.isAvailable() == oldItem.isAvailable())
        return false;

    m_files[index] = item;
    QString fileName = item.fileName();
    QString filePath = item.filePath();
    QString filePathLocal = item.filePathLocal();
    int idPlaylist = item.idPlaylist();
    int idFile = item.idFile();
    int idFormat = item.idFormat();
    bool isAvailable = item.isAvailable();
//    QString format = item.format();

    emit itemEdited(index, idFile, idPlaylist, fileName, filePath, filePathLocal, idFormat, isAvailable);
    return true;
}

void PlFiles::clear()
{
    if (m_files.size() > 0)
        m_files.clear();
}

void PlFiles::scroll(int index)
{
    if (m_files.isEmpty())
        m_curIdFile = 0;
    else
        m_curIdFile = m_files.at(index).idFile();
    emit filesScrolled(m_idPlaylist, m_curIdFile);
}

void PlFiles::addItem(const int &idFile, const int &idPlaylist, const QString &fileName, const QString &filePath,
                      const QString &filePathLocal, const int &idFormat, const bool &isAvailable, const QString &format)
{    
    PlFile newFile;    
    newFile.setIdFile(idFile);
    newFile.setIdPlaylist(idPlaylist);
    newFile.setFileName(fileName);
    newFile.setFilePath(filePath);
    newFile.setFilePathLocal(filePathLocal);
    newFile.setIdFormat(idFormat);
    newFile.setIsAvailable(isAvailable);
    newFile.setFormat(format);

    m_files.append(newFile);
//    emit listChanged();
}

int PlFiles::rowCount() const
{
    if (!m_files.isEmpty())
        return m_files.size();
    return 0;
}

QString PlFiles::fileName() const
{
    return m_files.at(0).fileName();
}

void PlFiles::setFilePath(int index, QString filePath)
{
    PlFile curFile = m_files.at(index);
    curFile.setFilePath(filePath);
    setItemAt(index, curFile);
    emit itemChanged(index);
}

void PlFiles::selectFile()
{    
//    QFileDialog flDlg(QApplication::);
//    flDlg.setFileMode(QFileDialog::ExistingFile);
//    flDlg.setNameFilter(tr("m3u playlists (*.m3u, *.m3u8)"));
//    flDlg.setViewMode(QFileDialog::Detail);
//    if (flDlg.exec()) {
//        m_filesName = flDlg.selectedFiles();
//        if (m_filesName.capacity() > 0)
//            m_fileName = m_filesName.at(0);
//        qDebug() << m_fileName;
//    }




}

QUrl PlFiles::getCurDir()
{
    if (m_sets) {
        return QUrl::fromLocalFile(m_sets->curPlDir());
    } else {
        return QUrl::fromLocalFile("");
    }
}

void PlFiles::setCurDir(const QUrl &curDir)
{
    qDebug() << "plfiles: setCurDir: " << curDir;
    if (m_sets) {
        qDebug() << "isLocal: " << curDir.isLocalFile();
        m_curDir = curDir.path();
        qDebug() << "curPlDir: " << m_curDir;
        m_sets->setCurPlDir(m_curDir);
    }
}

void PlFiles::setSets(const Settings &value)
{    
    m_sets = std::make_shared< Settings > (value);
}

int PlFiles::curIdFile() const
{
    return m_curIdFile;
}

int PlFiles::curIndex() const
{
    //
    return 0;
}

void PlFiles::open(const int &idPlaylist)
{
    qDebug() << "plfiles open id: " << idPlaylist;
    if (m_files.size() > 0) {
        emit selectItem(0);
        emit scroll(0);
    }
//    QString fileName;
//    QString filePath;
//    QString filePathLocal;
//    int idFile;
//    int idFormat;
//    bool isAvailable;
//    QString format;
//    this->m_idPlaylist = idPlaylist;

//    m_files.clear();

//    //adding default file/category "All"
////    if (idPlaylist == 0) {
////        fileName = tr("All");
////        filePath = filePathLocal = "";
////        idFile = 0;
////        this->addItem(&idFile, &idPlaylist, &fileName, &filePath, &filePathLocal, &idFormat, &isAvailable, &format);

//    //adding files added manually
//    m_fileGw->select(&idPlaylist);
//    if (*m_fileGw->rows() > 0) {
//        m_fileGw->data()->first();
//        do {
//            idFile = m_fileGw->data()->value("id_file").toInt();
//            fileName = m_fileGw->data()->value("file_name").toString();
//            filePath = m_fileGw->data()->value("file_path").toString();
//            filePathLocal = m_fileGw->data()->value("file_path_local").toString();
//            idFormat = m_fileGw->data()->value("id_format").toInt();
//            isAvailable = m_fileGw->data()->value("is_available").toBool();
//            format = m_fileGw->data()->value("format").toString();

//            qDebug() << "adding files: " << fileName << ". idFile: " << idFile;
//            this->addItem(&idFile, &idPlaylist, &fileName, &filePath, &filePathLocal, &idFormat, &isAvailable, &format);
//        } while(m_fileGw->data()->next());
//    }
//    qDebug() << "rowCountChanged(rowCount()); " << rowCount();
    emit rowCountChanged(rowCount());
}

void PlFiles::open(const bool &allFilesInPlaylist, const int &idFile)
{
    qDebug() << "PlFiles: open: all: " << allFilesInPlaylist << " idFile: " << idFile;
    if (allFilesInPlaylist) {
        emit filesScrolled(m_idPlaylist, 0);
    } else {
        emit filesScrolled(m_idPlaylist, idFile);
    }
}

void PlFiles::appendNewItem()
{
    emit beforeItemAppended();

    bool isAvailable {false};
    int idFormat {};

    PlFile item;
    item.setIsAvailable(isAvailable);
    item.setIdPlaylist(m_idPlaylist);
    item.setIdFormat(idFormat);

    m_files.append(item);
    qDebug() << "plFiles: appendNewItem()";
    emit itemAppended(m_files.size() - 1, m_idPlaylist, item.fileName(), item.filePath(), item.filePathLocal(), item.idFormat(), isAvailable);

    emit afterItemAppended();
    emit rowCountChanged(rowCount());
    emit selectItem(m_files.size() - 1);
}

void PlFiles::addItemFromLocalFile(int index, QUrl filePath)
{
    qDebug() << "PlFiles::addItemFromLocalFile: " << filePath;
    waitForId();
    auto id = m_files.at(index).idFile();
    QString newFilePath {m_sets->appPath() + "/temp/" + QString::number(id) + "/" + filePath.fileName()};

    m_plThr.copyFile(index, filePath, newFilePath, true);
    m_plThr.start();
}

void PlFiles::addItemFromBuffer(int index, QUrl filePath)
{
    qDebug() << "filepath: " << QUrl::fromLocalFile(filePath.path());

    QUrl locFile(QUrl::fromLocalFile(filePath.path()));
    qDebug() << "addItemFromBuffer: " << filePath.toLocalFile();
    if (locFile.isLocalFile()) {
        filePath = locFile;
        qDebug() << "addItemFromBuffer: is local file";
        addItemFromLocalFile(index, filePath);
    } else {
        qDebug() << "addItemFromBuffer: is not local file";
        addItemFromUrl(index, filePath);
    }

}

void PlFiles::addItemFromUrl(int index, QUrl fileUrl)
{
    waitForId();
    qDebug() << "plfiles: addItemFromUrl:";
    QString urlPath {fileUrl.toString()};

    qDebug() << "addItemFromUrl: " << fileUrl.fileName();
    int id = m_files.at(index).idFile();
    QString newPath = m_sets->appPath() + "/temp/" + QString::number(id) + "/" + fileUrl.fileName();

    QFileInfo fInf(newPath);
    QString fileExt = fInf.suffix();
    uint sender {1};
    emit checkFileExtension(index, urlPath, newPath, fileExt, sender);

//    m_fileLoader.doDownload(fileUrl, newPath);

}

void PlFiles::refresh(int index)
{
    if (index > -1) {
        QString inStr {"http"};
        QString filePath {m_files.at(index).filePath()};

        auto file = m_files.at(index);
        bool isAvailable {false};
        file.setIsAvailable(isAvailable);
        setItemAt(index, file);

        if (filePath.contains(inStr)) {
            qDebug() << "refreshing url file";
            refreshUrlFile(index, filePath);
        } else {
            qDebug() << "refresh local file";
            refreshLocalFile(index, filePath);
        }
    }
    emit itemChanged(index);
}

void PlFiles::refreshLocalFile(int index, QString filePath)
{
    QFile file(filePath);
    if (file.exists()) {
        QString newFilePath {m_files.at(index).filePathLocal()};
        QUrl url(filePath);
        m_plThr.refreshFile(index, url, newFilePath);
        m_plThr.start();
    }
}

void PlFiles::refreshUrlFile(int index, QString filePath)
{
    QUrl fileUrl(filePath);
    QString fullFilePath {m_files.at(index).filePath()};
    QString newFilePath {m_files.at(index).filePathLocal()};
    QString ext {m_files.at(index).format()};
    bool isValid {true};
    int idExt {m_files.at(index).idFormat()};
    downloadFileFromInternet(index, fullFilePath, newFilePath, ext, isValid, idExt);
}

void PlFiles::removeCurrentItem(int index)
{    
    if (index > -1 && index < m_files.size()) {
        emit beforeItemRemoved(index);

        int curIndex;
        if (index == m_files.size() - 1)
            curIndex = m_files.size() - 2;
        else
            curIndex = index;
        // deleting file from local Temp directory
        QString filePathTemp = m_files.at(index).filePathLocal();
        QFileInfo fInf(filePathTemp);
        if (fInf.exists()) {
            QFile fl(filePathTemp);
            fl.remove();
        }

        int id = m_files.at(index).idFile();
        m_files.removeAt(index);

        emit itemDeleted(index, id);
        emit afterItemRemoved();

        if (m_files.size() > 1 && curIndex == 0)
            emit selectItem(1);
        if (m_files.size() > 0)
            emit selectItem(curIndex);
    }
    qDebug() << "removeCurrentItem()";
    emit rowCountChanged(rowCount());
}

QString PlFiles::getClipboardString()
{
//    QClipboard *cbrd = QApplication::clipboard();
//    qDebug() << "clipboard: " << cbrd->text();
    QString clipText = QApplication::clipboard()->text();
    clipText.remove("\n");
    clipText.remove("\t");
    clipText.remove("\u202A");
    QChar before = QLatin1Char('\\');
    QChar after = QLatin1Char('/');

    clipText.replace(before, after);

    qDebug() << "clipboard: " << clipText;
    return clipText;
}

int PlFiles::idPlaylist() const
{
    return m_idPlaylist;
}

void PlFiles::setIdPlaylist(const int &idPlaylist)
{    
    m_idPlaylist = idPlaylist;
}

void PlFiles::initConnections()
{
    connect(&m_plThr, SIGNAL(checkFileExtension(int,QString&,QString&,QString,uint)),
            this, SIGNAL(checkFileExtension(int,QString&,QString&,QString,uint)));

    connect(this, SIGNAL(extensionChecked(int,QString&,QString&,QString,bool,int)),
            &m_plThr, SLOT(extensionChecked(int,QString&,QString&,QString,bool,int)));

    connect(&m_plThr, SIGNAL(fileCopied(QString,QString,QString,QString,int,bool,int)),
            this, SLOT(addCopiedFileInfo(QString,QString,QString,QString,int,bool,int)));
    connect(&m_plThr, SIGNAL(error(QString,QString,QString,QString,QString,int,bool,int)),
            this, SLOT(fileNotCopied(QString,QString,QString,QString,QString,int,bool,int)));

    connect(this, SIGNAL(itemFullyAdded()), &m_eventLoop, SLOT(quit()));

    connect(&m_fileLoader, SIGNAL(error(QString,QString,QString,QString,QString,int,bool,int)),
            this, SLOT(fileNotCopied(QString,QString,QString,QString,QString,int,bool,int)));
    connect(&m_fileLoader, SIGNAL(fileDownloaded(QString,QString,QString,QString,int,bool,int)),
            this, SLOT(addCopiedFileInfo(QString,QString,QString,QString,int,bool,int)));

    //    connect(&m_plThr, &FileWorkerThread::copyFinished, &m_plThr, &FileWorkerThread::deleteLater);
}

void PlFiles::waitForId()
{
    qDebug() << "PlFiles::waitForId()";
//    m_eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
}

void PlFiles::downloadFileFromInternet(const int &index, QString &fullFilePath, QString &newFilePath, const QString &extension,
                                       const bool &isValid, const int &idFormat)
{
    m_fileLoader.doDownload(newFilePath, index, fullFilePath, extension, isValid, idFormat, false);
}

void PlFiles::setNewItemIdFile(const int &index, const int &idFile)
{
    qDebug() << "PlFiles::setNewItemIdFile: " << idFile;
    auto newFileItem = m_files.at(index);
    newFileItem.setIdFile(idFile);
    m_curIdFile = idFile;
    setItemAt(index, newFileItem);

    scroll(index);
    emit itemChanged(index);    
    emit itemFullyAdded();
}

void PlFiles::addCopiedFileInfo(const QString &fileName, const QString &filePath, const QString &newFilePath,
                                const QString &extension, const int &idFormat, const bool &isAvailable, const int &index)
{
    qDebug() << "PlFiles::addCopiedFileInfo: " << index << " fileName: " << fileName;
    PlFile item = m_files.at(index);
    if (item.fileName() == "")
        item.setFileName(fileName);
    item.setFilePath(filePath);
    item.setFilePathLocal(newFilePath);
    item.setFormat(extension);
    item.setIdFormat(idFormat);
    item.setIsAvailable(isAvailable);
    setItemAt(index, item);

    emit itemChanged(index);  

    QFileInfo fInf(filePath);
    m_curDir = fInf.absolutePath();    
    m_sets->setCurPlDir(m_curDir);    

    emit fileFullyAdded(newFilePath);
}

void PlFiles::fileNotCopied(const QString &errorMsg, const QString &fileName, const QString &filePath, const QString &newFilePath,
                            const QString &extension, const int &idFormat, const bool &isAvailable, const int &index)
{
    m_plThr.disconnect();
    qDebug() << "PlFiles::fileNotCopied: " << errorMsg;
    addCopiedFileInfo(fileName, filePath, newFilePath, extension, idFormat, isAvailable, index);

}

void PlFiles::extensionChecked(const int &index, QString &fullFilePath, QString &newFilePath, const QString &extension,
                               const bool &isValid, const int &idFormat, const uint &sender)
{
    qDebug() << "plfiles: extensionChecked";
    if (isValid) {        
        if (sender == 0) {
            emit extensionChecked(index, fullFilePath, newFilePath, extension, isValid, idFormat);
        } else {
            downloadFileFromInternet(index, fullFilePath, newFilePath, extension, isValid, idFormat);
        }
    } else {
        if (sender == 0)
            emit extensionChecked(index, fullFilePath, newFilePath, extension, isValid, idFormat);
        QString msg {tr("File format is invalid!")};
        emit errorEmited(msg);
    }
}
