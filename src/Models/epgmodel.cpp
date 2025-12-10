#include "epgmodel.h"

#include "./Classes/epg.h"

#include <QDateTime>
#include <QUrl>
#include <QString>

EpgsModel::EpgsModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_epgs = new Epgs();
}

EpgsModel::~EpgsModel()
{
    m_epgs = nullptr;
}

int EpgsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !m_epgs)
        return 0;
    return m_epgs->items().size();
}

QVariant EpgsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return false;

    int idEpg {};
    QString naim {};
    QUrl url {};
    QDateTime dateTimeUpdate {};
    QDateTime dateTimeBegin {};
    QDateTime dateTimeEnd {};

    const Epg epg = m_epgs->items().at(index.row());

    switch (role) {
    case IdEpgRole:
        idEpg = epg.idEpg();
        return QVariant(idEpg);
    case NaimRole:
        naim = epg.naim();
        return QVariant(naim);
    case UrlRole:
        url = epg.url();
        return QVariant(url);
    case DateTimeUpdateRole:
        dateTimeUpdate = epg.dateTimeUpdate();
        return QVariant(dateTimeUpdate);
    case DateTimeBeginRole:
        dateTimeBegin = epg.dateTimeBegin();
        return QVariant(dateTimeBegin);
    case DateTimeEndRole:
        dateTimeEnd = epg.dateTimeEnd();
        return QVariant(dateTimeEnd);
    }
    return QVariant();
}

bool EpgsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!m_epgs)
        return false;

    int idEpg {};
    QString naim {};
    QUrl url {};
    QDateTime dateTimeUpdate {};
    QDateTime dateTimeBegin {};
    QDateTime dateTimeEnd {};

    Epg epg = m_epgs->items().at(index.row());

    switch (role) {
    case IdEpgRole:
        idEpg = value.toInt();
        epg.setIdEpg(idEpg);
        break;
    case NaimRole:
        naim = value.toString();
        epg.setNaim(naim);
        break;
    case UrlRole:
        url = epg.url();
        epg.setUrl(url);
        break;
    case DateTimeUpdateRole:
        dateTimeUpdate = epg.dateTimeUpdate();
        epg.setDateTimeUpdate(dateTimeUpdate);
        break;
    case DateTimeBeginRole:
        dateTimeBegin = epg.dateTimeBegin();
        epg.setDateTimeBegin(dateTimeBegin);
        break;
    case DateTimeEndRole:
        dateTimeEnd = epg.dateTimeEnd();
        epg.setDateTimeEnd(dateTimeEnd);
        break;
    }
    if (m_epgs->setItemAt(index.row(), epg)) {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }

    return false;
}

Qt::ItemFlags EpgsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> EpgsModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[IdEpgRole] = "id_channel";
    names[NaimRole] = "naim";
    names[UrlRole] = "url";
    names[DateTimeUpdateRole] = "date_time_update";
    names[DateTimeBeginRole] = "date_time_begin";
    names[DateTimeEndRole] = "date_time_end";

    return names;
}

Epgs *EpgsModel::epgs() const
{
    return m_epgs;
}

void EpgsModel::setEpgs(Epgs *newEpgs)
{
    beginResetModel();

    if (m_epgs)
        m_epgs->disconnect(this);

    m_epgs = newEpgs;

    if (m_epgs) {
        connect(m_epgs, &Epgs::beforeItemAppended, this, [=]() {
            const int index = m_epgs->items().size();
            beginInsertRows(QModelIndex(), index, index);
        });

        connect(m_epgs, &Epgs::afterItemAppended, this, [=]() {
            endInsertRows();
        });

        connect(m_epgs, &Epgs::beforeItemRemoved, this, [=](int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });

        connect(m_epgs, &Epgs::afterItemRemoved, this, [=]() {
            endRemoveRows();
        });
    }

    endResetModel();
    emit onEpgsChanged();
}
