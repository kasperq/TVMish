#ifndef PLAYLISTS_H
#define PLAYLISTS_H

#include <QObject>
#include <QVector>
//#include <QAbstractListModel>

#include "playlist.h"
#include "./Gateways/playlistgw.h"

class Playlists : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int rowCount READ rowCount)
    Q_PROPERTY(QString naim READ naim)
public:
    explicit Playlists(QObject *parent = nullptr);
    Playlists* operator=(const Playlists* orig);
    virtual ~Playlists();

    QVector<Playlist> items() const;
    bool setItemAt(int index, Playlist &item);
    void addItem(QString &naim, bool &isCurrent, int &idPlaylist, uint &num);
    void clear();

    int rowCount() const;
    QString naim() const;
    int curIdPlaylist() const;


signals:
    void beforeItemAppended();
    void afterItemAppended();      

    void beforeItemRemoved(int index);
    void afterItemRemoved();

    void selectItem(int index);
    void itemChanged(int index);
    void listChanged();
    void playlistScrolled(int idPlaylist);
    void itemAdded(const int &index, const QString &naim, const bool &isCurrent);
    void itemEdited(const int &idPlaylist, const QString &naim, const bool &isCurrent, const uint &num);
    void itemToDelete(const int &idPlaylist, const int &index);

public slots:
    void addItem();
    void removeItem(int index);
    void move(int index, QString type = "down");
    void indexChanged(int index);
    void setIsCurrent(int index, bool isCurr);

    //sets id_playlist and num fields to new added item
    void setItemIdPlaylistAndNum(const int &index, const int &idPlaylist, const uint &num);
    //removes item deleted from db
    void itemRemoved(const int &idPlaylist, const int &index);




private:
    QVector<Playlist> m_plLists;
};

#endif // PLAYLISTS_H
