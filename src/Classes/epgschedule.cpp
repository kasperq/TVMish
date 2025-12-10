#include "epgschedule.h"

EpgSchedule::EpgSchedule(QObject *parent)
    : QObject{parent}
{

}

EpgSchedule::~EpgSchedule()
{

}

QVector<EpgKart> EpgSchedule::items() const
{
    return m_kart;
}

bool EpgSchedule::setItemAt(int index, EpgKart &item)
{
    if (index < 0 || index >= m_kart.size())
        return false;

    const EpgKart &oldItem = m_kart.at(index);
    if (item.idKart() == oldItem.idKart()
            && item.idEpg() == oldItem.idEpg()
            && item.idChannel() == oldItem.idChannel()
            && item.epgCode() == oldItem.epgCode()
            && item.timeStart() == oldItem.timeStart()
            && item.timeEnd() == oldItem.timeEnd()
            && item.timeShift() == oldItem.timeShift()
            && item.title() == oldItem.title()
            && item.description() == oldItem.description()
            && item.iconUrl() == oldItem.iconUrl()
            && item.idLogo() == oldItem.idLogo()
            && item.category() == oldItem.category()
            && item.ageRating() == oldItem.ageRating())
        return false;

    m_kart[index] = item;
    int idKart = item.idKart();
    int idEpg = item.idEpg();
    int idChannel = item.idChannel();
    int epgCode = item.epgCode();
    QDateTime timeStart = item.timeStart();
    QDateTime timeEnd = item.timeEnd();
    int timeShift = item.timeShift();
    QString title = item.title();
    QString description = item.description();
    QUrl iconUrl = item.iconUrl();
    int idLogo = item.idLogo();
    QString category = item.category();
    QString ageRating = item.ageRating();

    emit itemEdited(index, idKart, idEpg, idChannel, epgCode, timeStart, timeEnd, timeShift, title, description, iconUrl,
                    idLogo, category, ageRating);
    return true;
}

void EpgSchedule::addItem(const int &idKart, const int &idEpg, const int &idChannel, const int &epgCode,
                          const QDateTime &timeStart, const QDateTime &timeEnd, const int &timeShift, const QString &title,
                          const QString &description, const QUrl &iconUrl, const int &idLogo, const QString &category,
                          const QString &ageRating)
{
    EpgKart newEpg;
    newEpg.setIdKart(idKart);
    newEpg.setIdEpg(idEpg);
    newEpg.setIdChannel(idChannel);
    newEpg.setEpgCode(epgCode);
    newEpg.setTimeStart(timeStart);
    newEpg.setTimeEnd(timeEnd);
    newEpg.setTimeShift(timeShift);
    newEpg.setTitle(title);
    newEpg.setDescription(description);
    newEpg.setIconUrl(iconUrl);
    newEpg.setIdLogo(idLogo);
    newEpg.setCategory(category);
    newEpg.setAgeRating(ageRating);

    m_kart.append(newEpg);
}

void EpgSchedule::clear()
{
    if (m_kart.size() > 0)
        m_kart.clear();
    emit rowCountChanged(rowCount());
}

int EpgSchedule::rowCount() const
{
    return m_kart.isEmpty() ? 0 : m_kart.size();
}

void EpgSchedule::initConnections()
{

}

int EpgSchedule::curIdEpgKart() const
{
    return m_curIdKart;
}

void EpgSchedule::open()
{
    if (m_kart.size() > 0)
        emit selectItem(0);
    emit rowCountChanged(rowCount());
}

void EpgSchedule::scroll(const int &index)
{
    if (m_kart.isEmpty()) {
        m_curIdKart = 0;
    }
    else {
        m_curIdKart = m_kart.at(index).idKart();
    }
}

void EpgSchedule::setNewIdKart(const int &index, const int &newId)
{
    auto epg = m_kart.at(index);
    epg.setIdEpg(newId);

    m_kart[index] = epg;
    emit itemChanged(index);
}

void EpgSchedule::appendNewItem()
{
    emit beforeItemAppended();

    EpgKart item;
    item.setTimeStart(QDateTime::currentDateTime());
    item.setTimeEnd(QDateTime::currentDateTime());
    item.setTimeShift(3);
    item.setIconUrl(QUrl(""));

    m_kart.append(item);
    emit itemAppended(m_kart.size() - 1, item.idKart(), item.idEpg(), item.idChannel(), item.epgCode(), item.timeStart(),
                      item.timeEnd(), item.timeShift(), item.title(), item.description(), item.iconUrl(), item.idLogo(),
                      item.category(), item.ageRating());

    emit afterItemAppended();
    emit rowCountChanged(rowCount());
    emit selectItem(m_kart.size() - 1);
}
