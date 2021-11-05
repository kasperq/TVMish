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
    explicit Playlists(PlaylistGW *plGw);
    Playlists(QObject *parent = nullptr);
    Playlists* operator=(const Playlists* orig);
    virtual ~Playlists();

    QVector<Playlist> items() const;
    bool setItemAt(int index, Playlist &item);
    void addItem(QString *naim, bool *isCurrent, int *idPlaylist, uint *num);

    int rowCount() const;
    QString naim() const;


signals:
    void beforeItemAppended();
    void afterItemAppended();      

    void beforeItemRemoved(int index);
    void afterItemRemoved();

    void selectItem(int index);
    void itemChanged(int index);
    void playlistScrolled(int idPlaylist);


public slots:
    void addItem();
    void removeItem(int index);
    void move(int index, QString type = "down");
    void indexChanged(int index);
    void setIsCurrent(int index, bool isCurr);






private:
    QVector<Playlist> m_plLists;
    PlaylistGW *m_plGw;

};

#endif // PLAYLISTS_H
