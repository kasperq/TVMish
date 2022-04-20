#include "plfilemodel.h"

PlFilesModel::PlFilesModel()
{    
    m_files = new PlFiles();
}

PlFilesModel::~PlFilesModel()
{
    m_files = nullptr;
}

int PlFilesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !m_files)
        return 0;
    return m_files->items().size();
}

QVariant PlFilesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return false;
    QString fileName;
    QString filePath;
    QString filePathLocal;
    int idPlaylist;
    int idFile;
    int idFormat;
    bool isAvailable;
    QString format;

    const PlFile plFile = m_files->items().at(index.row());    
//    qDebug() << "model: " << plFile.fileName();
    switch (role) {
    case FileNameRole:
        fileName = plFile.fileName();
        return QVariant(fileName);
    case FilePathRole:
        filePath = plFile.filePath();
        return QVariant(filePath);
    case FilePathLocalRole:
        filePathLocal = plFile.filePathLocal();
        return QVariant(filePathLocal);
    case IdPlaylistRole:
        idPlaylist = plFile.idPlaylist();
        return QVariant(idPlaylist);
    case IdFileRole:
        idFile = plFile.idFile();
        return QVariant(idFile);
    case IdFormatRole:
        idFormat = plFile.idFormat();
        return QVariant(idFormat);
    case IsAvailableRole:
        isAvailable = plFile.isAvailable();
        return QVariant(isAvailable);
    case FormatRole:
        format = plFile.format();
        return QVariant(format);
    }
    return QVariant();
}

bool PlFilesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!m_files)
        return false;

    QString fileName;
    QString filePath;
    QString filePathLocal;
    int idPlaylist;
    int idFile;
    int idFormat;
    bool isAvailable;
    QString format;

    PlFile plFile = m_files->items().at(index.row());    
    switch (role) {
    case FileNameRole:        
        fileName = value.toString();
        plFile.setFileName(fileName);
        break;
    case FilePathRole:
        filePath = value.toString();
        plFile.setFilePath(filePath);
        break;
    case FilePathLocalRole:
        filePathLocal = value.toString();
        plFile.setFilePathLocal(filePathLocal);
        break;
    case IdPlaylistRole:
        idPlaylist = value.toInt();
        plFile.setIdPlaylist(idPlaylist);
        break;
    case IdFileRole:
        idFile = value.toInt();
        plFile.setIdFile(idFile);
        break;
    case IdFormatRole:
        idFormat = value.toInt();
        plFile.setIdFormat(idFormat);
        break;
    case IsAvailableRole:
        isAvailable = value.toBool();
        plFile.setIsAvailable(isAvailable);
        break;
    case FormatRole:
        format = value.toString();
        plFile.setFormat(format);
        break;
    }

    if (m_files->setItemAt(index.row(), plFile)) {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }

    return false;
}

Qt::ItemFlags PlFilesModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> PlFilesModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[FileNameRole] = "file_name";
    names[FilePathRole] = "file_path";
    names[FilePathLocalRole] = "file_path_local";
    names[IdPlaylistRole] = "id_playlist";
    names[IdFileRole] = "id_file";
    names[IdFormatRole] = "id_format";
    names[IsAvailableRole] = "is_available";
    names[FormatRole] = "format";

    return names;
}

PlFiles *PlFilesModel::files() const
{
//    qDebug() << "model: m_files.size: " << m_files->items().size();
    return m_files;
}

void PlFilesModel::setFiles(PlFiles *files)
{    
    beginResetModel();

    if (m_files)
        m_files->disconnect(this);

    m_files = files;

//    qDebug() << "model: " << m_files->items().size();

    if (m_files) {
        connect(m_files, &PlFiles::beforeItemAppended, this, [=]() {
            const int index = m_files->items().size();
            beginInsertRows(QModelIndex(), index, index);
        });

        connect(m_files, &PlFiles::afterItemAppended, this, [=]() {
            endInsertRows();
        });

        connect(m_files, &PlFiles::beforeItemRemoved, this, [=](int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });

        connect(m_files, &PlFiles::afterItemRemoved, this, [=]() {
            endRemoveRows();
        });

        //connect(m_plLists, &Playlists::itemAdded, this, [=](int intdex) {});
    }

    endResetModel();
}
