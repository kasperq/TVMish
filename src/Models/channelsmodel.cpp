#include "channelsmodel.h"

#include "Classes/channel.h"

#include <QUrl>

ChannelsModel::ChannelsModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_channels = new Channels();
}

ChannelsModel::~ChannelsModel()
{
    m_channels = nullptr;
}

int ChannelsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !m_channels)
        return 0;
    return m_channels->items().size();
}

QVariant ChannelsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return false;
    int idChannel {};
    QString naim {};
    QString url {};
    int idPlaylist {};
    int idFile {};
    int idCategory {};
    bool isAvailable {};
    QString categoryNaim {};
    int numFile {};
    int numPlaylist {};
    int archDays {};
    int idLogo {};
    QUrl logoPath {};
    bool isFavorite {};

    const Channel channel = m_channels->items().at(index.row());

    switch (role) {
    case IdChannelRole:
        idChannel = channel.idChannel();
        return QVariant(idChannel);
    case NaimRole:
        naim = channel.naim();
        return QVariant(naim);
    case UrlRole:
        url = channel.url();
        return QVariant(url);
    case IdPlaylistRole:
        idPlaylist = channel.idPlaylist();
        return QVariant(idPlaylist);
    case IdFileRole:
        idFile = channel.idFile();
        return QVariant(idFile);
    case IdCategoryRole:
        idCategory = channel.idCategory();
        return QVariant(idCategory);
    case IsAvailableRole:
        isAvailable = channel.isAvailable();
        return QVariant(isAvailable);
    case CategoryNaimRole:
        categoryNaim = channel.categoryNaim();
        return QVariant(categoryNaim);
    case NumFileRole:
        numFile = channel.numFile();
        return QVariant(numFile);
    case NumPlaylistRole:
        numPlaylist = channel.numPlaylist();
        return QVariant(numPlaylist);
    case ArchDaysRole:
        archDays = channel.archDays();
        return QVariant(archDays);
    case IdLogoRole:
        idLogo = channel.idLogo();
        return QVariant(idLogo);
    case LogoPathRole:
        logoPath = channel.logoPath();
        return QVariant(logoPath);
    case IsFavoriteRole:
        isFavorite = channel.isFavorite();
        return QVariant(isFavorite);
    }
    return QVariant();
}

bool ChannelsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!m_channels)
        return false;

    int idChannel {};
    QString naim {};
    QString url {};
    int idPlaylist {};
    int idFile {};
    int idCategory {};
    bool isAvailable {};
    QString categoryNaim {};
    int numFile {};
    int numPlaylist {};
    int archDays {};
    int idLogo {};
    QUrl logoPath {};
    bool isFavorite {};

    Channel channel = m_channels->items().at(index.row());
    switch (role) {
    case IdChannelRole:
        idChannel = value.toInt();
        channel.setIdChannel(idChannel);
        break;
    case NaimRole:
        naim = value.toString();
        channel.setNaim(naim);
        break;
    case UrlRole:
        url = value.toString();
        channel.setUrl(url);
        break;
    case IdPlaylistRole:
        idPlaylist = value.toInt();
        channel.setIdPlaylist(idPlaylist);
        break;
    case IdFileRole:
        idFile = value.toInt();
        channel.setIdFile(idFile);
        break;
    case IdCategoryRole:
        idCategory = value.toInt();
        channel.setIdCategory(idCategory);
        break;
    case IsAvailableRole:
        isAvailable = value.toBool();
        channel.setIsAvailable(isAvailable);
        break;
    case CategoryNaimRole:
        categoryNaim = value.toString();
        channel.setCategoryNaim(categoryNaim);
        break;
    case NumFileRole:
        numFile = value.toInt();
        channel.setNumFile(numFile);
        break;
    case NumPlaylistRole:
        numPlaylist = value.toInt();
        channel.setNumPlaylist(numPlaylist);
        break;
    case ArchDaysRole:
        archDays = value.toInt();
        channel.setArchDays(archDays);
        break;
    case IdLogoRole:
        idLogo = value.toInt();
        channel.setIdLogo(idLogo);
        break;
    case LogoPathRole:
        logoPath = value.toUrl();
        channel.setLogoPath(logoPath);
        break;
    case IsFavoriteRole:
        isFavorite = value.toBool();
        channel.setIsFavorite(isFavorite);
        break;
    }

    if (m_channels->setItemAt(index.row(), channel)) {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }

    return false;
}

Qt::ItemFlags ChannelsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> ChannelsModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[IdChannelRole] = "id_channel";
    names[NaimRole] = "naim";
    names[UrlRole] = "url";
    names[IdPlaylistRole] = "id_playlist";
    names[IdFileRole] = "id_file";
    names[IdCategoryRole] = "id_category";
    names[IsAvailableRole] = "is_available";
    names[CategoryNaimRole] = "category_naim";
    names[NumFileRole] = "num_file";
    names[NumPlaylistRole] = "num_playlist";
    names[ArchDaysRole] = "archive_days";
    names[IdLogoRole] = "id_logo";
    names[LogoRole] = "logo";
    names[LogoPathRole] = "logo_path";
    names[IsFavoriteRole] = "is_favorite";

    return names;
}

Channels *ChannelsModel::channels() const
{
    return m_channels;
}

void ChannelsModel::setChannels(Channels *newChannels)
{
    beginResetModel();

    if (m_channels)
        m_channels->disconnect(this);

    m_channels = newChannels;

    if (m_channels) {
        connect(m_channels, &Channels::beforeItemAppended, this, [=]() {
            const int index = m_channels->items().size();
            beginInsertRows(QModelIndex(), index, index);
        });

        connect(m_channels, &Channels::afterItemAppended, this, [=]() {
            endInsertRows();
        });

        connect(m_channels, &Channels::beforeItemRemoved, this, [=](int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });

        connect(m_channels, &Channels::afterItemRemoved, this, [=]() {
            endRemoveRows();
        });
    }

    endResetModel();
    emit onChannelsChanged();
}
