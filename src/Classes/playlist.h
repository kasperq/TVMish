#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>

class Playlist
{
public:
    Playlist();
    virtual ~Playlist();

    int idPlaylist() const;
    void setIdPlaylist(const int& value);

    bool isCurrent() const;
    void setIsCurrent(const bool& value);

    QString naim() const;
    void setNaim(const QString& value);

    unsigned int num() const;
    void setNum(const uint& value);

private:
    int m_idPlaylist;
    bool m_isCurrent;
    QString m_naim;
    unsigned int m_num;

};

#endif // PLAYLIST_H
