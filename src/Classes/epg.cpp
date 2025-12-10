#include "epg.h"

Epg::Epg()
{

}

Epg::~Epg()
{

}

int Epg::idEpg() const
{
    return m_idEpg;
}

void Epg::setIdEpg(const int &newIdEpg)
{
    m_idEpg = newIdEpg;
}

const QUrl &Epg::url() const
{
    return m_url;
}

void Epg::setUrl(const QUrl &newUrl)
{
    m_url = newUrl;
}

const QString &Epg::naim() const
{
    return m_naim;
}

void Epg::setNaim(const QString &newNaim)
{
    m_naim = newNaim;
}

const QDateTime &Epg::dateTimeUpdate() const
{
    return m_dateTimeUpdate;
}

void Epg::setDateTimeUpdate(const QDateTime &newDateTimeUpdate)
{
    m_dateTimeUpdate = newDateTimeUpdate;
}

const QDateTime &Epg::dateTimeEnd() const
{
    return m_dateTimeEnd;
}

void Epg::setDateTimeEnd(const QDateTime &newDateTimeEnd)
{
    m_dateTimeEnd = newDateTimeEnd;
}

const QDateTime &Epg::dateTimeBegin() const
{
    return m_dateTimeBegin;
}

void Epg::setDateTimeBegin(const QDateTime &newDateTimeBegin)
{
    m_dateTimeBegin = newDateTimeBegin;
}
