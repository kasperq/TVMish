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

void Playlist::setIdPlaylist(const int& value)
{
    m_idPlaylist = value;
}

bool Playlist::isCurrent() const
{
    return m_isCurrent;
}

void Playlist::setIsCurrent(const bool& value)
{
    m_isCurrent = value;
}

QString Playlist::naim() const
{
    return m_naim;
}

void Playlist::setNaim(const QString& value)
{
    m_naim = value;
}

unsigned int Playlist::num() const
{
    return m_num;
}

void Playlist::setNum(const uint& value)
{
    m_num = value;
}
