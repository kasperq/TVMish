#include "settings.h"

#include <QtConcurrent/QtConcurrent>
#include <QFuture>

Settings::Settings()
{

}

Settings::Settings(const Settings &value)
{

}

Settings::Settings(const QSqlDatabase &db) : m_db(db)
{
//    SetsThread m_setsThr {m_db};
//    m_setsThr.start();
    qDebug() << "db1: " << db;
    getSets();
    qDebug() << "db2: " << db;
}

Settings::~Settings()
{
//    m_setsThr.quit();
//    m_setsThr.wait();
}

QString Settings::appPath()
{
    if (m_appPath.trimmed() == "")
        getSets();
    return m_appPath;
}

void Settings::setAppPath(const QString &value)
{
    m_appPath = value;
    if (m_rows == 0)
        insertSets();
    else
        modifySets();
    getSets();
}

QString Settings::curPlDir()
{
    if (m_curPlDir.trimmed() == "")
        this->getSets();
    return m_curPlDir;
}

void Settings::setCurPlDir(const QString &value)
{
    m_curPlDir = value;
    if (m_rows == 0)
        insertSets();
    else
        modifySets();
    qDebug() << "sets changed";
    getSets();
    qDebug() << "sets getted";
}

void Settings::setDB(const QSqlDatabase &value)
{
    m_db = value;
}

void Settings::countRows()
{
    m_rows = 0;
    q_select.first();
    do {
        if (q_select.isValid())
            ++m_rows;
    } while (q_select.next());
}

void Settings::getSets()
{
    qDebug() << "getSets " << m_db;
    q_select = QSqlQuery(m_db);
    q_select.prepare("select settings.app_dir, settings.current_pl_dir, settings.id_sets from settings ");
    q_select.exec();
    countRows();
    m_appPath = "";
    m_curPlDir = "";
    q_select.first();
    if (m_rows > 0) {
        m_appPath = q_select.value("app_dir").toString();
        m_curPlDir = q_select.value("current_pl_dir").toString();
        m_id = q_select.value("id_sets").toInt();
    }
}

void Settings::modifySets()
{
    qDebug() << "sets modify";

//    m_setsThr.modify(m_appPath, m_curPlDir);

//    q_temp = QSqlQuery(m_db);
//    q_temp.prepare("update settings "
//                    "set app_dir = :app_dir, "
//                    "current_pl_dir = :current_pl_dir "
//                    "where id_sets = :id_sets ");
//    q_temp.bindValue(":app_dir", m_appPath);
//    q_temp.bindValue(":current_pl_dir", m_curPlDir);
//    q_temp.bindValue(":id_sets", m_id);
//    q_temp.exec();
//    qDebug() << "sets modify ended";
//    q_temp.finish();
}

void Settings::insertSets()
{
    qDebug() << "insertSets";
    q_temp = QSqlQuery(m_db);
    q_temp.prepare("insert into SETTINGS "
                    "(app_dir, current_pl_dir) "
                    "values (:app_dir, :current_pl_dir) ");
    q_temp.bindValue(":app_dir", m_appPath);
    q_temp.bindValue(":current_pl_dir", m_curPlDir);
    q_temp.exec();
    q_temp.finish();
}

void Settings::modifyThr(QString &appPath, QString &curPlDir)
{
    qDebug() << "sets modify";
    q_temp = QSqlQuery(m_db);
    q_temp.prepare("update settings "
                    "set app_dir = :app_dir, "
                    "current_pl_dir = :current_pl_dir "
                    "where id_sets = :id_sets ");
    q_temp.bindValue(":app_dir", appPath);
    q_temp.bindValue(":current_pl_dir", curPlDir);
    q_temp.exec();
    qDebug() << "sets modify ended";
    q_temp.finish();
}
