#include "epgs.h"

#include "epg.h"

Epgs::Epgs(QObject *parent)
    : QObject{parent}
{    
}

Epgs::~Epgs()
{

}

QVector<Epg> Epgs::items() const
{
    return m_epgs;
}

bool Epgs::setItemAt(int index, Epg &item)
{
    if (index < 0 || index >= m_epgs.size())
        return false;

    const Epg &oldItem = m_epgs.at(index);
    if (item.idEpg() == oldItem.idEpg()
            && item.naim() == oldItem.naim()
            && item.url() == oldItem.url()
            && item.dateTimeUpdate() == oldItem.dateTimeUpdate()
            && item.dateTimeBegin() == oldItem.dateTimeBegin()
            && item.dateTimeEnd() == oldItem.dateTimeEnd())
        return false;

    m_epgs[index] = item;
    int idEpg = item.idEpg();
    QString naim = item.naim();
    QUrl url = item.url();
    QDateTime dateTimeUpdate = item.dateTimeUpdate();
    QDateTime dateTimeBegin = item.dateTimeBegin();
    QDateTime dateTimeEnd = item.dateTimeEnd();

    emit itemEdited(index, idEpg, naim, url, dateTimeUpdate, dateTimeBegin, dateTimeEnd);
    return true;
}

void Epgs::addItem(const int &idEpg, const QString &naim, const QUrl &url, const QDateTime &dateTimeUpdate,
                   const QDateTime &dateTimeBegin, const QDateTime &dateTimeEnd)
{
    Epg newEpg;
    newEpg.setIdEpg(idEpg);
    newEpg.setNaim(naim);
    newEpg.setUrl(url);
    newEpg.setDateTimeUpdate(dateTimeUpdate);
    newEpg.setDateTimeBegin(dateTimeBegin);
    newEpg.setDateTimeEnd(dateTimeEnd);

    m_epgs.append(newEpg);
}

void Epgs::clear()
{
    if (m_epgs.size() > 0)
        m_epgs.clear();
    emit rowCountChanged(rowCount());
}

int Epgs::rowCount() const
{
    if (!m_epgs.isEmpty())
        return m_epgs.size();
    return 0;
}

void Epgs::initConnections()
{

}

void Epgs::open()
{
    if (m_epgs.size() > 0)
        emit selectItem(0);
    emit rowCountChanged(rowCount());
}

void Epgs::scroll(const int &index)
{
    if (m_epgs.isEmpty()) {
        m_curIdEpg = 0;
        m_curUrl.clear();
    }
    else {
        m_curIdEpg = m_epgs.at(index).idEpg();
        m_curUrl = QUrl(m_epgs.at(index).url());
    }
}

void Epgs::setNewIdEpg(const int &index, const int &newId)
{
    auto epg = m_epgs.at(index);
    epg.setIdEpg(newId);

    m_epgs[index] = epg;
    emit itemChanged(index);
}

void Epgs::appendNewItem()
{
    emit beforeItemAppended();

    Epg item;
    item.setDateTimeUpdate(QDateTime::currentDateTime());
    item.setDateTimeBegin(QDateTime::currentDateTime());
    item.setDateTimeEnd(QDateTime::currentDateTime());
    item.setNaim("");
    item.setUrl(QUrl(""));

    m_epgs.append(item);
    emit itemAppended(m_epgs.size() - 1, item.naim(), item.url(), item.dateTimeUpdate(), item.dateTimeBegin(), item.dateTimeEnd());

    emit afterItemAppended();
    emit rowCountChanged(rowCount());
    emit selectItem(m_epgs.size() - 1);
}

void Epgs::update(const int &index, const QUrl &fileUrl)
{

}

void Epgs::deleteEpg(const int &index, const int &idEpg)
{
    if (idEpg > -1 && idEpg < m_epgs.size()) {
        emit beforeItemRemoved(index);

        int curIndex;
        if (index == m_epgs.size() - 1)
            curIndex = m_epgs.size() - 2;
        else
            curIndex = index;

        m_epgs.erase(std::remove_if(m_epgs.begin(), m_epgs.end(), [idEpg] (Epg epg) { return idEpg == epg.idEpg(); }));

        emit epgDeleted(index, idEpg);
        emit afterItemRemoved();

        if (m_epgs.size() > 1 && curIndex == 0)
            emit selectItem(1);
        if (m_epgs.size() > 0)
            emit selectItem(curIndex);
    }
}

const QUrl &Epgs::curUrl() const
{
    return m_curUrl;
}

int Epgs::curIdEpg() const
{
    return m_curIdEpg;
}
