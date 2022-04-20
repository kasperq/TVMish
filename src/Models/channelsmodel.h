#ifndef CHANNELSMODEL_H
#define CHANNELSMODEL_H

#include <QAbstractListModel>
#include <QObject>

#include "Classes/channels.h"

class ChannelsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Channels *list READ channels WRITE setChannels NOTIFY onChannelsChanged)
public:
    explicit ChannelsModel(QObject *parent = nullptr);
    virtual ~ChannelsModel();

    enum {
        IdChannelRole = Qt::UserRole,
        NaimRole,
        UrlRole,
        IdPlaylistRole,
        IdFileRole,
        IdCategoryRole,
        NumFileRole,
        NumPlaylistRole,
        ArchDaysRole,
        IdLogoRole,
        LogoRole,
        CategoryNaimRole,
        IsAvailableRole,
        LogoPathRole,
        IsFavoriteRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    Channels *channels() const;
    void setChannels(Channels *newChannels);

signals:
    void onChannelsChanged();

public slots:

private:
    Channels *m_channels;
};

#endif // CHANNELSMODEL_H
