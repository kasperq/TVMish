#include "epgkart.h"

EpgKart::EpgKart()
{

}

EpgKart::~EpgKart()
{

}

int EpgKart::idKart() const
{
    return m_idKart;
}

void EpgKart::setIdKart(const int &newIdKart)
{
    m_idKart = newIdKart;
}

int EpgKart::idEpg() const
{
    return m_idEpg;
}

void EpgKart::setIdEpg(const int &newIdEpg)
{
    m_idEpg = newIdEpg;
}

int EpgKart::idChannel() const
{
    return m_idChannel;
}

void EpgKart::setIdChannel(const int &newIdChannel)
{
    m_idChannel = newIdChannel;
}

int EpgKart::epgCode() const
{
    return m_epgCode;
}

void EpgKart::setEpgCode(const int &newEpgCOde)
{
    m_epgCode = newEpgCOde;
}

const QDateTime &EpgKart::timeStart() const
{
    return m_timeStart;
}

void EpgKart::setTimeStart(const QDateTime &newTimeStart)
{
    m_timeStart = newTimeStart;
}

const QDateTime &EpgKart::timeEnd() const
{
    return m_timeEnd;
}

void EpgKart::setTimeEnd(const QDateTime &newTimeEnd)
{
    m_timeEnd = newTimeEnd;
}

int EpgKart::timeShift() const
{
    return m_timeShift;
}

void EpgKart::setTimeShift(const int &newTimeShift)
{
    m_timeShift = newTimeShift;
}

const QString &EpgKart::title() const
{
    return m_title;
}

void EpgKart::setTitle(const QString &newTitle)
{
    m_title = newTitle;
}

const QString &EpgKart::description() const
{
    return m_description;
}

void EpgKart::setDescription(const QString &newDescription)
{
    m_description = newDescription;
}

const QUrl &EpgKart::iconUrl() const
{
    return m_iconUrl;
}

void EpgKart::setIconUrl(const QUrl &newIconUrl)
{
    m_iconUrl = newIconUrl;
}

int EpgKart::idLogo() const
{
    return m_idLogo;
}

void EpgKart::setIdLogo(const int &newIdLogo)
{
    m_idLogo = newIdLogo;
}

const QString &EpgKart::category() const
{
    return m_category;
}

void EpgKart::setCategory(const QString &newCategory)
{
    m_category = newCategory;
}

const QString &EpgKart::ageRating() const
{
    return m_ageRating;
}

void EpgKart::setAgeRating(const QString &newAgeRating)
{
    m_ageRating = newAgeRating;
}
