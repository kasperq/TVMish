#include "settings.h"

Settings::Settings()
{

}

Settings::Settings(const QSqlDatabase &db) : m_db(db)
{
    getSets();
}

Settings::~Settings()
{

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
    getSets();
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
    q_select = QSqlQuery(m_db);
    q_select.prepare("select settings.app_dir, settings.current_pl_dir from settings ");
    q_select.exec();
    countRows();
    m_appPath = "";
    m_curPlDir = "";
    q_select.first();
    if (m_rows > 0) {
        m_appPath = q_select.value("app_dir").toString();
        m_curPlDir = q_select.value("current_pl_dir").toString();
    }
}

void Settings::modifySets()
{
    q_temp = QSqlQuery(m_db);
    q_temp.prepare("update settings "
                    "set app_dir = :app_dir, "
                    "current_pl_dir = :current_pl_dir ");
    q_temp.bindValue(":app_dir", m_appPath);
    q_temp.bindValue(":current_pl_dir", m_curPlDir);
    q_temp.exec();
}

void Settings::insertSets()
{
    q_temp = QSqlQuery();
    q_temp.prepare("insert into SETTINGS "
                    "(app_dir, current_pl_dir) "
                    "values (:app_dir, :current_pl_dir) ");
    q_temp.bindValue(":app_dir", m_appPath);
    q_temp.bindValue(":current_pl_dir", m_curPlDir);
    q_temp.exec();
}
