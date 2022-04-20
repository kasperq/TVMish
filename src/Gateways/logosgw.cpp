#include "logosgw.h"

#include "./DB/dbst.h"

#include <QDebug>
#include <QSqlError>

LogosGW::LogosGW(QObject *parent)
    : QObject{parent}
{

}

LogosGW::~LogosGW()
{

}

QSqlQuery *LogosGW::data() const
{
    return const_cast<QSqlQuery*>(&q_select);
}

int LogosGW::rows() const
{
    return m_rows;
}

void LogosGW::select(const int &idLogo)
{
    DBst::getInstance().startTransDBdef();
    q_select = QSqlQuery(DBst::getInstance().db_def());
    QString queryStr;
    q_select.clear();
    queryStr = "select logos.CHANNEL_NAME, logos.EPG_CODE, logos.ID_LOGO, logos.LOGO_PATH, logos.LOGO_SMALL_PATH, "
               "logos.CHANNEL_NAME_LOWER "
               "from logos "
               "where logos.ID_LOGO = :id_logo ";

    q_select.prepare(queryStr);
    q_select.bindValue(":id_logo", idLogo);

    if (q_select.exec()) {
        q_select.first();
        calcRowCount();
    } else {
        qDebug() << "LogosGW::select error: " << q_select.lastError();
    }
    emit selected(idLogo);
}

void LogosGW::insert(const int &index, const QString &channelName, const QString &logoPath, const QString &logoPathSmall, const QString &epgCode)
{
    DBst::getInstance().startTransDB();
    q_insert = QSqlQuery(DBst::getInstance().db_def());
    q_insert.clear();
    q_insert.prepare("insert into LOGOS "
                     "(ID_LOGO, CHANNEL_NAME, LOGO_PATH, LOGO_SMALL_PATH, EPG_CODE, CHANNEL_NAME_LOWER) "
                     "values (:id_logo, :channel_name, :logo_path, :logo_small_path, :epg_code, :channel_name_lower) ");
    q_insert.bindValue(":channel_name", channelName);
    q_insert.bindValue(":logo_path", logoPath);
    q_insert.bindValue(":logo_small_path", logoPathSmall);
    q_insert.bindValue(":epg_code", epgCode);
    m_newId = getMaxId() + 1;
    q_insert.bindValue(":id_logo", m_newId);
    q_insert.bindValue(":channel_name_lower", channelName.toLower());

    if (q_insert.exec()) {
        DBst::getInstance().commitDbDef();
        q_insert.finish();
        emit inserted(index, m_newId, logoPath);
    }
}

void LogosGW::modify(const int &index, const int &idLogo, const QString &channelName, const QString &logoPath,
                     const QString &logoPathSmall, const QString &epgCode)
{
    DBst::getInstance().startTransDB();
    q_modify = QSqlQuery(DBst::getInstance().db_def());
    q_modify.clear();
    q_modify.prepare("update LOGOS "
                     "set "
                     "CHANNEL_NAME = channel_name, "
                     "LOGO_PATH = :logo_path, "
                     "LOGO_SMALL_PATH = :logo_small_path, "
                     "EPG_CODE = :epg_code, "
                     "CHANNEL_NAME_LOWER = :channel_name_lower "
                     "where id_logo = :id_logo ");
    q_modify.bindValue(":channel_name", channelName);
    q_modify.bindValue(":logo_path", logoPath);
    q_modify.bindValue(":logo_small_path", logoPathSmall);
    q_modify.bindValue(":epg_code", epgCode);
    q_modify.bindValue(":id_logo", idLogo);
    q_modify.bindValue(":channel_name_lower", channelName.toLower());

    if (q_modify.exec()) {
        DBst::getInstance().commitDbDef();
        q_modify.finish();
        emit modified(index, idLogo);
    }
}

void LogosGW::modifyPath(const int &idLogo, const QString &logoPath, const QString &logoPathSmall)
{
    DBst::getInstance().startTransDB();
    q_modify = QSqlQuery(DBst::getInstance().db_def());
    q_modify.clear();
    q_modify.prepare("update LOGOS "
                     "set "
                     "LOGO_PATH = :logo_path, "
                     "LOGO_SMALL_PATH = :logo_small_path "
                     "where id_logo = :id_logo ");
    q_modify.bindValue(":logo_path", logoPath);
    q_modify.bindValue(":logo_small_path", logoPathSmall);
    q_modify.bindValue(":id_logo", idLogo);

    if (q_modify.exec()) {
        DBst::getInstance().commitDbDef();
        q_modify.finish();
    }
}

void LogosGW::deleteRecord(const int &index, const int &idLogo)
{
    DBst::getInstance().startTransDBdef();
    q_delete = QSqlQuery(DBst::getInstance().db_def());
    q_delete.prepare("delete from LOGOS where id_logo = :id_logo ");
    q_delete.bindValue(":id_logo", idLogo);

    if (q_delete.exec()) {
        DBst::getInstance().commitDbDef();
        q_delete.finish();
        emit deleted(index, idLogo);
    }
}

bool LogosGW::find(const int &index, const QString &channelName)
{
    DBst::getInstance().startTransDBdef();
    q_temp = QSqlQuery(DBst::getInstance().db_def());
    q_temp.prepare("select logos.id_logos, logos.channel_name, logos.LOGO_PATH, logos.LOGO_SMALL_PATH "
                   "from logos "
                   "where trim(logos.CHANNEL_NAME_LOWER) like trim(\"%" + channelName.toLower() + "%\") ");
    q_temp.exec();

    q_temp.first();
    int id {};
    bool isFound {false};
    QString logoPath {};
    QString logoPathSmall {};
    if (q_temp.isValid()) {
        id = q_temp.value("id_category").toInt();
        logoPath = q_temp.value("logo_path").toString();
        logoPathSmall = q_temp.value("logo_small_path").toString();
        isFound = true;
        emit found(index, isFound, id, logoPath, logoPathSmall);
        q_temp.finish();
        return true;
    }

    q_temp.finish();
    emit found(index, isFound, id, logoPath, logoPathSmall);
    return false;
}

void LogosGW::calcRowCount()
{
    m_rows = 0;
    if (q_select.first()) {
        do {
            ++m_rows;
        }
        while (q_select.next());
        q_select.first();
    }
}

int LogosGW::getMaxId()
{
    DBst::getInstance().startTransDBdef();
    q_temp = QSqlQuery(DBst::getInstance().db_def());
    q_temp.prepare("select coalesce(max(logos.id_logo),0) max_id from logos ");
    try {
        if (q_temp.exec()) {
            q_temp.first();
            int maxId {};
            if (q_temp.isValid())
                maxId = q_temp.value(0).toInt();
            q_temp.finish();
            return maxId;
        } else {
            qDebug() << "channelGW::getMaxId error: " << q_temp.lastError();
        }
    } catch (...) {
        qDebug() << "error getMaxId: " << q_temp.lastError();
    }
    return 0;
}
