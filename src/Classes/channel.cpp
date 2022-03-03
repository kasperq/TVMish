#include "channel.h"

Channel::Channel()
{

}

Channel::~Channel()
{

}

void Channel::clear()
{
    m_idChannel = 0;
    m_naim.clear();
    m_url.clear();
    m_idFile = 0;
    m_idPlaylist = 0;
    m_idCategory = 0;
    m_numFile = 0;
    m_numPlaylist = 0;
    m_archDays = 0;
    m_idLogo = 0;
}

int Channel::idChannel() const
{
    return m_idChannel;
}

void Channel::setIdChannel(const int &newIdChannel)
{
    m_idChannel = newIdChannel;
}

QString Channel::naim() const
{
    return m_naim;
}

void Channel::setNaim(const QString &newNaim)
{
    m_naim = newNaim;
}

QString Channel::url() const
{
    return m_url;
}

void Channel::setUrl(const QString &newUrl)
{
    m_url = newUrl;
}

int Channel::idFile() const
{
    return m_idFile;
}

void Channel::setIdFile(const int &newIdFile)
{
    m_idFile = newIdFile;
}

int Channel::idPlaylist() const
{
    return m_idPlaylist;
}

void Channel::setIdPlaylist(const int &newIdPlaylist)
{
    m_idPlaylist = newIdPlaylist;
}

int Channel::idCategory() const
{
    return m_idCategory;
}

void Channel::setIdCategory(const int &newIdCategory)
{
    m_idCategory = newIdCategory;
}

int Channel::numFile() const
{
    return m_numFile;
}

void Channel::setNumFile(const int &newNumFile)
{
    m_numFile = newNumFile;
}

int Channel::numPlaylist() const
{
    return m_numPlaylist;
}

void Channel::setNumPlaylist(const int &newNumPlaylist)
{
    m_numPlaylist = newNumPlaylist;
}

int Channel::archDays() const
{
    return m_archDays;
}

void Channel::setArchDays(const int &newArchDays)
{
    m_archDays = newArchDays;
}

int Channel::idLogo() const
{
    return m_idLogo;
}

void Channel::setIdLogo(const int &newIdLogo)
{
    m_idLogo = newIdLogo;
}
