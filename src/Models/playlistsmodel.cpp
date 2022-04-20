#include "playlistsmodel.h"
#include "./Classes/playlists.h"

PlaylistsModel::PlaylistsModel(QObject *parent) : QAbstractListModel(parent)
{
    qDebug() << "constructor without params PlaylistsModel";
    m_plLists = new Playlists();
}

PlaylistsModel::~PlaylistsModel()
{
    m_plLists = nullptr;
}

int PlaylistsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !m_plLists)
        return 0;
    return m_plLists->items().size();
}

QVariant PlaylistsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return false;
    QString naim;
    bool isCurrent;
    int idPlaylist;
    uint num;

    const Playlist plList = m_plLists->items().at(index.row());
    switch (role) {
    case NaimRole:
        naim = plList.naim();
        return QVariant(naim);
    case IsCurrentRole:
        isCurrent = plList.isCurrent();
        return QVariant(isCurrent);
    case idPlaylistRole:
        idPlaylist = plList.idPlaylist();
        return QVariant(idPlaylist);
    case NumRole:
        num = plList.num();
        return QVariant(num);
    }
    return QVariant();
}

bool PlaylistsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!m_plLists)
        return false;
    QString naim;
    bool isCurrent;
    int idPlaylist;
    uint num;

    Playlist plList = m_plLists->items().at(index.row());
    switch (role) {
    case NaimRole:
        naim = value.toString();
        plList.setNaim(naim);
        break;
    case IsCurrentRole:
        isCurrent = value.toBool();
        plList.setIsCurrent(isCurrent);
        break;
    case idPlaylistRole:
        idPlaylist = value.toInt();
        plList.setIdPlaylist(idPlaylist);
        break;
    case NumRole:
        num = value.toUInt();
        plList.setNum(num);
        break;
    }

    if (m_plLists->setItemAt(index.row(), plList)) {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }

    return false;
}

Qt::ItemFlags PlaylistsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> PlaylistsModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[NaimRole] = "naim";
    names[IsCurrentRole] = "is_current";
    names[idPlaylistRole] = "id_playlist";
    names[NumRole] = "num";
    return names;
}

Playlists *PlaylistsModel::plLists() const
{
    return m_plLists;
}

void PlaylistsModel::setPlLists(Playlists *plLists)
{
    beginResetModel();

    if (m_plLists)
        m_plLists->disconnect(this);
    m_plLists = plLists;

    if (m_plLists) {
        connect(m_plLists, &Playlists::beforeItemAppended, this, [=]() {
            const int index = m_plLists->items().size();
            beginInsertRows(QModelIndex(), index, index);
        });

        connect(m_plLists, &Playlists::afterItemAppended, this, [=]() {
            endInsertRows();
        });

        connect(m_plLists, &Playlists::beforeItemRemoved, this, [=](int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });

        connect(m_plLists, &Playlists::afterItemRemoved, this, [=]() {
            endRemoveRows();
        });

        //connect(m_plLists, &Playlists::itemAdded, this, [=](int intdex) {});
    }

    endResetModel();
}
