#include "playlists.h"

#include "./Models/playlistsmodel.h"

Playlists::Playlists(PlaylistGW *plGw) : m_plGw(plGw)
{
    qDebug() << "constructor with params Playlists";
}

Playlists::Playlists(QObject *parent) : QObject(parent)
{
    qDebug() << "constructor without params Playlists";
}

Playlists *Playlists::operator=(const Playlists *orig)
{
    m_plLists.clear();
    QString naim;
    bool isCurrent;
    int idPlaylist;
    uint num;
    QVector< Playlist >::iterator pI;
    qDebug() << "===";
    for(pI = orig->items().begin(); pI != orig->items().end(); ++pI) {
        naim = pI->naim();
        isCurrent = pI->isCurrent();
        idPlaylist = pI->idPlaylist();
        num = pI->num();
        addItem(&naim, &isCurrent, &idPlaylist, &num);
    }
    return this;
}

Playlists::~Playlists()
{

}

QVector<Playlist> Playlists::items() const
{
    return m_plLists;
}

bool Playlists::setItemAt(int index, Playlist &item)
{
    if (index < 0 || index >= m_plLists.size())
        return false;

    const Playlist &oldlItem = m_plLists.at(index);
    if (item.idPlaylist() == oldlItem.idPlaylist()
        && item.isCurrent() == oldlItem.isCurrent()
        && item.naim() == oldlItem.naim()
        && item.num() == oldlItem.num())
        return false;

    m_plLists[index] = item;
    QString naim = item.naim();
    bool isCurrent = item.isCurrent();
    int idPlaylist = item.idPlaylist();
    uint num = item.num();

    m_plGw->modify(&idPlaylist, &naim, &isCurrent, &num);

    return true;
}

void Playlists::addItem(QString *naim, bool *isCurrent, int *idPlaylist, uint *num)
{
    Playlist newPl;
    newPl.setIdPlaylist(idPlaylist);
    newPl.setIsCurrent(isCurrent);
    newPl.setNaim(naim);
    newPl.setNum(num);

    m_plLists.append(newPl);

//    qDebug() << "pl. append() " << *naim;
}

void Playlists::addItem()
{
    emit beforeItemAppended();

    Playlist item;
    bool isCurrent = false;
    uint num = 0;
    item.setIsCurrent(&isCurrent);
    item.setNum(&num);

    m_plGw->insert(&item);

    m_plLists.append(item);
//    qDebug() << "pl. append()";
    emit afterItemAppended();
//    qDebug() << "selectItem(curIndex): " << m_plLists.size() - 1;
    emit selectItem(m_plLists.size() - 1);
}

void Playlists::removeItem(int index)
{
    emit beforeItemRemoved(index);

    int curIndex;
    if (index == m_plLists.size() - 1)
        curIndex = m_plLists.size() - 2;
    else
        curIndex = index;

    int id = m_plLists.at(index).idPlaylist();
    m_plGw->deleteRecord(&id);
    m_plLists.removeAt(index);    

    emit afterItemRemoved();
//    qDebug() << "selectItem(curIndex): " << curIndex;
    if (m_plLists.size() > 1 && curIndex == 0)
        emit selectItem(1);
    if (m_plLists.size() > 0)
        emit selectItem(curIndex);
}

void Playlists::move(int index, QString type)
{
    Playlist pl;
    QString naim;
    bool isCurrent;
    int idPlaylist;
    uint num;
    qsizetype curIndex;
    qsizetype moveToIndex;
    bool moving = false;
    int multipleSign = 1;

    if (index != -1) {
        if (type.toLower() == "up") {
            if (index > 0) {
                curIndex = index;
                moveToIndex = --index;
                multipleSign = -1;

                moving = true;
            }
        } else {
            if (index < m_plLists.size() - 1) {
                curIndex = index;
                moveToIndex = ++index;
                multipleSign = 1;

                moving = true;
            }
        }
    }

    if (moving) {
        naim = m_plLists.at(curIndex).naim();
        idPlaylist = m_plLists.at(curIndex).idPlaylist();
        isCurrent = m_plLists.at(curIndex).isCurrent();
        num = m_plLists.at(curIndex).num() + multipleSign;

        pl.setIdPlaylist(&idPlaylist);
        pl.setNaim(&naim);
        pl.setIsCurrent(&isCurrent);
        pl.setNum(&num);

        setItemAt(curIndex, pl);

        naim = m_plLists.at(moveToIndex).naim();
        idPlaylist = m_plLists.at(moveToIndex).idPlaylist();
        isCurrent = m_plLists.at(moveToIndex).isCurrent();
        num = m_plLists.at(moveToIndex).num() - multipleSign;

        pl.setIdPlaylist(&idPlaylist);
        pl.setNaim(&naim);
        pl.setIsCurrent(&isCurrent);
        pl.setNum(&num);

        setItemAt(moveToIndex, pl);

        m_plLists.swapItemsAt(curIndex, moveToIndex);

        emit itemChanged(moveToIndex);
        emit selectItem(moveToIndex);
    }
}

void Playlists::indexChanged(int index)
{
    emit playlistScrolled(m_plLists.at(index).idPlaylist());
}

void Playlists::setIsCurrent(int index, bool isCurr)
{
    QList<Playlist>::iterator plIt;
    int ind {0};
    int id {0};
    QString naim {};
    uint num {};
    bool newIsCurr {false};
    for (plIt = m_plLists.begin(); plIt != m_plLists.end(); ++plIt) {
        if (plIt->isCurrent() && ind != index) {
            plIt->setIsCurrent(&newIsCurr);
            id = plIt->idPlaylist();
            naim = plIt->naim();
            num = plIt->num();

            m_plGw->modify(&id, &naim, &newIsCurr, &num);
        }
        if (ind == index) {
            plIt->setIsCurrent(&isCurr);
            id = plIt->idPlaylist();
            naim = plIt->naim();
            num = plIt->num();

            m_plGw->modify(&id, &naim, &isCurr, &num);
        }
        ++ind;
    }

    emit itemChanged(index);
//    emit selectItem(index);
}

int Playlists::rowCount() const
{
    return m_plLists.size();
}

QString Playlists::naim() const
{
    return m_plLists.at(0).naim();
}
