#ifndef PLAYLISTSMODEL_H
#define PLAYLISTSMODEL_H

#include <QAbstractListModel>

#include "./Classes/playlists.h"
#include "./Gateways/playlistgw.h"

class PlaylistsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Playlists *list READ plLists WRITE setPlLists)
    Q_PROPERTY(int rowCount READ rowCount)

public:
    explicit PlaylistsModel(PlaylistGW *plGw, Playlists *plLists);
    PlaylistsModel(QObject *parent = nullptr);
    PlaylistsModel(PlaylistGW *plGw);

    enum {
        NaimRole = Qt::UserRole,
        IsCurrentRole,
        idPlaylistRole,
        NumRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    Playlists *plLists() const;
    void setPlLists(Playlists *plLists);

private:
    PlaylistGW *m_plGw;
    Playlists *m_plLists;
};

#endif // PLAYLISTSMODEL_H
