#include "settings.h"

#include <QtConcurrent/QtConcurrent>
#include <QFuture>

#include "DB/dbst.h"

#include <QSqlResult>

Settings::Settings()
{
    getSets();
}

Settings::Settings(const Settings &value, QObject *parent) : QObject(parent)
{
//    *this = value;
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
    if (m_rows == 0) {
        insertSets();
    } else {
        modifySets();
    }
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

QString &Settings::logoPath()
{
    if (m_logoPath.trimmed() == "")
        this->getSets();
    return m_logoPath;
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
//    if (DBst::getInstance().isDbThrOpened()) {
        q_select = QSqlQuery(DBst::getInstance().db_def());
        q_select.prepare("select settings.app_dir, settings.current_pl_dir, settings.id_sets, settings.logo_path from settings ");

//        QFuture<QSqlQuery> future = DBst::getInstance().execAndGetQuery(q_select);
//        qDebug() << "";
//        future.then([this](QSqlQuery query) {
//            q_select = query;
//        });
//        q_select = future.result();
//        qDebug() << " ";
        q_select.exec();
        countRows();
        m_appPath = "";
        m_curPlDir = "";
        q_select.first();                
        if (m_rows > 0) {            
            m_appPath = q_select.value("app_dir").toString();
            m_curPlDir = q_select.value("current_pl_dir").toString();
            m_id = q_select.value("id_sets").toInt();            
            m_logoPath = q_select.value("logo_path").toString();
        }
        q_select.finish();
//    }
}

void Settings::modifySets()
{
//    qDebug() << "sets modify: " << m_appPath << " curPlDir: " << m_curPlDir << " id_sets: " << m_id;
    q_temp = QSqlQuery(DBst::getInstance().db_def());
    q_temp.prepare("update settings "
                    "set app_dir = :app_dir, "
                    "current_pl_dir = :current_pl_dir, "
                    "logo_path = :logo_path "
                    "where id_sets = :id_sets ");
    q_temp.bindValue(":app_dir", m_appPath);
    q_temp.bindValue(":current_pl_dir", m_curPlDir);
    q_temp.bindValue(":id_sets", m_id);
    q_temp.bindValue(":logo_path", m_logoPath);
    q_temp.exec();
//    DBst::getInstance().execAndCheck(q_temp).then([this](bool result) {
//        if (result)
    DBst::getInstance().commitDbDef();
            q_temp.finish();
//    });
}

void Settings::insertSets()
{
//    qDebug() << "insertSets";
//    q_temp = QSqlQuery(m_db);
    q_temp = QSqlQuery(DBst::getInstance().db_def());
    q_temp.prepare("insert into SETTINGS "
                    "(app_dir, current_pl_dir, logo_path) "
                    "values (:app_dir, :current_pl_dir, :logo_path) ");
    q_temp.bindValue(":app_dir", m_appPath);
    q_temp.bindValue(":current_pl_dir", m_curPlDir);
    q_temp.bindValue(":logo_path", m_logoPath);
//    DBst::getInstance().execAndCheck(q_temp).then([this](bool result) {
//        if (result)
//            q_temp.finish();
//    });
    q_temp.exec();
    DBst::getInstance().commitDbDef();
    q_temp.finish();
}

void Settings::getSetsContinuation()
{
    countRows();
    m_appPath = "";
    m_curPlDir = "";
    q_select.first();
    if (m_rows > 0) {
        m_appPath = q_select.value("app_dir").toString();
        m_curPlDir = q_select.value("current_pl_dir").toString();
        m_id = q_select.value("id_sets").toInt();
        m_logoPath  = q_select.value("logo_path").toString();
    }
}
