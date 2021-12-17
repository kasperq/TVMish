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

    m_plThr.start();
//    m_sets = std::make_shared<Settings>();
}

PlFiles::PlFiles(Settings &sets) : m_sets(std::make_shared< Settings> (sets))
{
    qDebug() << "constructor without params PlFiles";
    m_idPlaylist = 1;
    initConnections();

    m_plThr.start();
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
    m_files.clear();
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
}

int PlFiles::rowCount() const
{
    return m_files.size();
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

int PlFiles::curIndex() const
{
    //
    return 0;
}

void PlFiles::open(int idPlaylist)
{
//    qDebug() << "plfiles open id: " << idPlaylist;
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
    emit itemAppended(m_files.size() - 1, m_idPlaylist, item.fileName(), item.filePath(), item.filePathLocal(), item.idFormat(), isAvailable);

    emit afterItemAppended();
    emit selectItem(m_files.size() - 1);
    emit rowCountChanged(rowCount());
}

void PlFiles::addItemFromLocalFile(int index, QUrl filePath)
{  
    m_plThr.copyFile(index, filePath);
//    if (filePath.isLocalFile()) {
//        m_curDir = filePath.toLocalFile();
//        QFileInfo fInf(filePath.toLocalFile());
//        QString fileExt = fInf.completeSuffix();
//        bool isValid = isFormatValid(fileExt);
//        if (isValid) {
//            int idFormat {/*m_fileGw->getFormatId(&fileExt)*/};
//            QString plFilePath = m_curDir;
//            QString fName = fInf.fileName();
//            bool isAvailable = true;

//            m_curDir = fInf.absolutePath();
//            m_sets->setCurPlDir(m_curDir);

//            QString newFilePath = m_sets->appPath() + "/temp/" + fName;

//            copyLocalFile(plFilePath, newFilePath);

//            PlFile item = m_files.at(index);
//            if (item.fileName() == "")
//                item.setFileName(fName);
//            item.setFilePath(plFilePath);
//            item.setFilePathLocal(newFilePath);
//            item.setFormat(fileExt);
//            item.setIdFormat(idFormat);
//            item.setIsAvailable(isAvailable);
//            setItemAt(index, item);

//            emit itemChanged(index);
//        }
//    }
}

void PlFiles::addItemFromUrl(int index, QUrl fileUrl)
{


}

void PlFiles::removeCurrentItem(int index)
{
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
    return clipText;
}

int PlFiles::idPlaylist() const
{
    return m_idPlaylist;
}

void PlFiles::setIdPlaylist(int idPlaylist)
{
    m_idPlaylist = idPlaylist;
}

bool PlFiles::isFormatValid(QString &format)
{
//    return m_fileGw->findFormat(format);
}

void PlFiles::copyLocalFile(QString &oldPath, QString &newPath)
{
    QFile oldFile(oldPath);
    QFileInfo fInf(newPath);

    QDir newDir;
    if (newDir.mkpath(fInf.absolutePath())) {
        connect(&m_plThr, SIGNAL(finished()), this, SLOT(copyFinished()));
        m_plThr.copyFile(oldPath, newPath);
        qDebug() << "start_copy: " << QTime::currentTime();
        m_plThr.start();
    }
}

void PlFiles::initConnections()
{
    connect(&m_plThr, SIGNAL(checkFileExtension(int,QString&,QString)),
            this, SIGNAL(checkFileExtension(int,QString&,QString)));
    connect(this, SIGNAL(extensionChecked(int,QString&,QString,bool,int,QString)),
            &m_plThr, SLOT(extensionChecked(int,QString&,QString,bool,int,QString)));
    connect(&m_plThr, SIGNAL(fileCopied(QString,QString,QString,QString,int,bool,int)),
            this, SLOT(addCopiedFileInfo(QString,QString,QString,QString,int,bool,int)));
    connect(&m_plThr, SIGNAL(error(QString)), this, SLOT(fileNotCopied(QString)));
}

void PlFiles::copyFinished()
{
    qDebug() << "copy_finished: " << QTime::currentTime();
}

void PlFiles::setNewItemIdFile(const int &index, const int &idFile)
{
    auto newFileItem = m_files.at(index);
    newFileItem.setIdFile(idFile);

    setItemAt(index, newFileItem);
    emit itemChanged(index);
}

void PlFiles::addCopiedFileInfo(const QString &fileName, const QString &filePath, const QString &newFilePath,
                                const QString &extension, const int &idFormat, const bool &isAvailable, const int &index)
{
    qDebug() << "addCopiedFileInfo " << index;
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

    QFileInfo fInf(fileName);
    m_curDir = fInf.absolutePath();
    m_sets->setCurPlDir(m_curDir);
}

void PlFiles::fileNotCopied(const QString &errorMsg)
{
    qDebug() << "File not copied: " << errorMsg;
}

void PlFiles::extensionChecked(const int &index, QString &fullFilePath, const QString &extension,
                               const bool &isValid, const int &idFormat)
{
    if (isValid) {
        QString appPath;
        if (m_sets)
            appPath = m_sets->appPath();
        emit extensionChecked(index, fullFilePath, extension, isValid, idFormat, appPath);
    } else {
        QString msg {tr("File format is invalid!")};
        emit errorEmited(msg);
    }
}
