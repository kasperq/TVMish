#include "dbconnpool.h"

#include <QDebug>

QMutex DBConnPool::m_mutex;
QAtomicPointer<DBConnPool> DBConnPool::m_instance = nullptr;
QWaitCondition DBConnPool::m_waitCondition;

void DBConnPool::closeAll()
{
    QMutexLocker locker(&m_mutex);
    delete m_instance;
    m_instance = nullptr;
}

QSqlDatabase DBConnPool::openConnection()
{
    DBConnPool &pool = DBConnPool::getInstance();
    QString connName;
    QMutexLocker locker(&m_mutex);
    int connCount = pool.unusedConnNames.size() + pool.usedConnNames.size();

    for (int i {};
         i < pool.m_maxWaitTime
         && pool.unusedConnNames.size() == 0
         && connCount == pool.m_maxConnectionCount;
         i += pool.m_waitInterval) {
        m_waitCondition.wait(&m_mutex, pool.m_waitInterval);
        connCount = pool.unusedConnNames.size() + pool.usedConnNames.size();
    }

    if (pool.unusedConnNames.size() > 0) {
        connName = pool.unusedConnNames.dequeue();
    } else if (connCount < pool.m_maxConnectionCount) {
        connName = QString("Connection-%1").arg(connCount + 1);
    } else {
        return QSqlDatabase();
    }

    QSqlDatabase db = pool.createConnection(connName);
    if (db.isOpen()) {
        pool.usedConnNames.enqueue(connName);
    }
    return db;
}

void DBConnPool::closeConnection(QSqlDatabase connection)
{
    DBConnPool &pool = DBConnPool::getInstance();
    QString connName = connection.connectionName();

    if (pool.usedConnNames.contains(connName)) {
        QMutexLocker locker(&m_mutex);
        pool.usedConnNames.removeOne(connName);
        pool.unusedConnNames.enqueue(connName);
        m_waitCondition.wakeOne();
    }
}

DBConnPool::~DBConnPool()
{
    foreach(QString connectionName, usedConnNames) {
        QSqlDatabase::removeDatabase(connectionName);
    }
    foreach(QString connectionName, unusedConnNames) {
        QSqlDatabase::removeDatabase(connectionName);
    }
}

DBConnPool &DBConnPool::getInstance()
{
#ifndef Q_ATOMIC_POINTER_TEST_AND_SET_IS_ALWAYS_NATIVE
    if (!QAtomicPointer<DBConnPool>::isTestAndSetNative())
        qDebug() << "Error: TestAndSetNative not supported!";
#endif
    {
        QMutexLocker locker(&m_mutex);
        if (m_instance.testAndSetOrdered(nullptr, nullptr)) {
            m_instance.testAndSetOrdered(nullptr, new DBConnPool);
        }
        return *m_instance.loadAcquire();
    }
}

QSqlDatabase DBConnPool::createConnection(const QString &connName)
{
    if (QSqlDatabase::contains(connName)) {
        QSqlDatabase dbOld = QSqlDatabase::database(connName);
        if (m_testOnBorrow) {
            QSqlQuery query(m_testSql, dbOld);
            if (query.lastError().type() != QSqlError::NoError && !dbOld.open()) {
                return QSqlDatabase();
            }
        }
        return dbOld;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase(m_dbType, connName);
    db.setDatabaseName(m_dbName);
    if (!db.open()) {
        qDebug() << "Open db error: " << db.lastError().text();
        return QSqlDatabase();
    }
    return db;
}

DBConnPool::DBConnPool()
{
    QFile dbFile;
    QString fileName;
#ifdef QT_DEBUG
    fileName = "d:/Projects/qt/TVmish/db/TVDB.db3";
#else
    fileName = QGuiApplication::applicationDirPath().toStdString() + "/TVDB.db3";
#endif
    dbFile.setFileName(fileName);
    if (!dbFile.exists()) {
        qWarning("DB missing!");
    }
    dbFile.close();

    m_dbName = fileName;

    m_dbType = "QSQLITE";
    m_testOnBorrow = true;
    m_testSql = "select 1";
    m_maxWaitTime = 1000;
    m_waitInterval = 200;
    m_maxConnectionCount = 15;
}
