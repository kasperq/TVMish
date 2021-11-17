#include "database.h"

#include <QtSql/QSqlError>
#include <QtCore>

//DBConnPool DataBase::m_pool;

DataBase::DataBase(QObject *parent) : QObject(parent)
{

}

DataBase::~DataBase()
{
//    if (m_db->isOpen())
//        m_db->close();
}

bool DataBase::connect()
{
//    QFile dbFile;
//    QString fileName;
//#ifdef QT_DEBUG
//    fileName = "d:/Projects/qt/TVmish/db/TVDB.db3";
//#else
//    fileName = QGuiApplication::applicationDirPath().toStdString() + "/TVDB.db3";
//#endif
//    dbFile.setFileName(fileName);
//    if (!dbFile.exists()) {
//        qWarning("DB missing!");
//        return false;
//    }
//    dbFile.close();

//    m_db = QSqlDatabase::addDatabase("QSQLITE");

//    if (!m_db.isValid()) {
//        qWarning("Cannot add db: %s", qPrintable(m_db.lastError().text()));
//        return false;
//    }
//    m_db.setDatabaseName(fileName);
//    if (!m_db.open()) {
//        qWarning("Cannot open db: %s", qPrintable(m_db.lastError().text()));
//        return false;
//    }
//    for(int i = 0; i < m_db.tables(QSql::Tables).count(); ++i)
//        qDebug() << "first table is " << m_db.tables(QSql::Tables).at(i);
    return true;
}

bool DataBase::disconnect()
{
    m_db.close();
    return true;
}

bool DataBase::disconnect(QSqlDatabase &db)
{
    DBConnPool::closeConnection(db);
//    m_pool.closeConnection(db);
    return true;
}

QSqlDatabase DataBase::db() const
{
    return DBConnPool::openConnection();
//    return m_pool.openConnection();
//    return m_db;
}
