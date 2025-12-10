#include "epggw.h"

#include "./DB/dbst.h"

#include <QSqlError>

EpgGW::EpgGW(QObject *parent)
    : QObject{parent}
{

}

EpgGW::~EpgGW()
{

}

QSqlQuery *EpgGW::data() const
{
    return const_cast<QSqlQuery*>(&q_query);
}

int EpgGW::rows() const
{
    return m_rows;
}

void EpgGW::select()
{
    DBst::getInstance().startTransDBdef();
    q_query = QSqlQuery(DBst::getInstance().db_def());
    QString queryStr;
    q_query.clear();
    queryStr = "select epg.ID_EPG, epg.NAIM, epg.URL, epg.DATE_TIME_UPDATE, epg.DATE_TIME_BEGIN, epg.DATE_TIME_END "
               "from epg ";

    q_query.prepare(queryStr);

    if (q_query.exec()) {
        q_query.first();
        calcRowCount();
    } else {
        qDebug() << "EpgGW::select error: " << q_query.lastError();
    }
    emit selected();
}

void EpgGW::insert(const int &index, const QString &naim, const QUrl &url, const QDateTime &dateTimeUpdate,
                   const QDateTime &dateTimeBegin, const QDateTime &dateTimeEnd)
{
    DBst::getInstance().startTransDB();
    qDebug() << "EpgGW: insert: " << naim;
    q_query = QSqlQuery(DBst::getInstance().db_def());
    q_query.clear();
    q_query.prepare("insert into EPG "
                     "(ID_EPG, NAIM, URL, DATE_TIME_UPDATE, DATE_TIME_BEGIN, DATE_TIME_END) "
                     "values (:id_epg, :naim, :url, :date_time_update, :date_time_begin, :date_time_end) ");
    q_query.bindValue(":naim", naim);
    q_query.bindValue(":url", url.toString());
    q_query.bindValue(":date_time_update", dateTimeUpdate);
    q_query.bindValue(":date_time_begin", dateTimeBegin);
    q_query.bindValue(":date_time_end", dateTimeEnd);
    q_query.bindValue(":id_epg", m_newId);


    DBst::getInstance().execAndCheck(q_query).then([this, naim, index](bool result) {
        qDebug() << "EpgGW: inserted: " << result << " naim: " << naim;
        if (result) {
            save();
            q_query.finish();
            qDebug() << "EpgGW: inserted: finished";
            emit inserted(index, m_newId);
        }
    });
}

void EpgGW::modify(const int &index, const int &idEpg, const QString &naim, const QUrl &url,
                   const QDateTime &dateTimeUpdate, const QDateTime &dateTimeBegin, const QDateTime &dateTimeEnd)
{
    DBst::getInstance().startTransDBdef();
    qDebug() << "url: " << url.path();
    q_query = QSqlQuery(DBst::getInstance().db_def());
    q_query.prepare("update EPG "
                     "set "
                     "ID_EPG = :id_epg, "
                     "NAIM = :naim, "
                     "URL = :url, "
                     "DATE_TIME_UPDATE = :date_time_update, "
                    "DATE_TIME_BEGIN = :date_time_begin, "
                     "DATE_TIME_END = :date_time_end "
                     "where id_epg = :id_epg ");
    q_query.bindValue(":id_epg", idEpg);
    q_query.bindValue(":naim", naim);
    q_query.bindValue(":url", url.toString());
    q_query.bindValue(":date_time_update", dateTimeUpdate);
    q_query.bindValue(":date_time_begin", dateTimeBegin);
    q_query.bindValue(":date_time_end", dateTimeEnd);

    if (q_query.exec()) {
        save();
        q_query.finish();
        emit modified(index);
    }
}

void EpgGW::deleteRecord(const int &index, const int &idEpg)
{
    DBst::getInstance().startTransDBdef();
    q_query = QSqlQuery(DBst::getInstance().db_def());
    q_query.prepare("delete from EPG where id_epg = :id_epg ");
    q_query.bindValue(":id_epg", idEpg);

    if (q_query.exec()) {
        save();
        q_query.finish();
        emit deleted(index, idEpg);
    }
}

void EpgGW::save()
{
    bool res = DBst::getInstance().commitDbDef();
    qDebug() << "EpgGW: save() : commited: " << res;
}

void EpgGW::calcRowCount()
{
    m_rows = 0;
    if (q_query.first()) {
        do {
            ++m_rows;
        }
        while (q_query.next());
        q_query.first();
    }
}

int EpgGW::getMaxId()
{
    DBst::getInstance().startTransDBdef();
    q_temp = QSqlQuery(DBst::getInstance().db_def());
    q_temp.prepare("select coalesce(max(epg.id_epg),0) max_id from epg ");
    try {
        if (q_temp.exec()) {
            q_temp.first();
            int maxId {};
            if (q_temp.isValid())
                maxId = q_temp.value(0).toInt();
            q_temp.finish();
            return maxId;
        } else {
            qDebug() << "EpgGW::getMaxId error: " << q_temp.lastError();
        }
    } catch (...) {
        qDebug() << "error getMaxId: " << q_temp.lastError();
    }
    return 0;
}
