#include "playlist.h"

Playlist::Playlist()
{

}

Playlist::~Playlist()
{

}

int Playlist::idPlaylist() const
{
    return m_idPlaylist;
}

void Playlist::setIdPlaylist(int* idPlaylist)
{
    m_idPlaylist = *idPlaylist;
}

bool Playlist::isCurrent() const
{
    return m_isCurrent;
}

void Playlist::setIsCurrent(bool* isCurrent)
{
    m_isCurrent = *isCurrent;
}

QString Playlist::naim() const
{
    return m_naim;
}

void Playlist::setNaim(QString* value)
{
    m_naim = *value;
}

unsigned int Playlist::num() const
{
    return m_num;
}

void Playlist::setNum(unsigned int *num)
{
    m_num = *num;
}
