#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>

class Playlist
{
public:
    Playlist();
    virtual ~Playlist();

    int idPlaylist() const;
    void setIdPlaylist(int* idPlaylist);

    bool isCurrent() const;
    void setIsCurrent(bool* isCurrent);

    QString naim() const;
    void setNaim(QString* value);

    unsigned int num() const;
    void setNum(unsigned int *num);

private:
    int m_idPlaylist;
    bool m_isCurrent;
    QString m_naim;
    unsigned int m_num;

};

#endif // PLAYLIST_H
