#include "dbst.h"

#include <QFile>
#include <QDebug>
#include <QSqlError>

//#include <QtConcurrent>
//#include <QFuture>
#include <variant>


#include <QTime>

DBst::DBst(QObject *parent) : QObject(parent)
{
    initDbSets();
}

DBst::DBst()
{
    initDbSets();
}

DBst::~DBst()
{
    qDebug() << "~DBSt()";
}

QFuture<bool> DBst::isPrepared()
{
    qDebug() << "Dbst isPrepared()";
    return QtConcurrent::run(&m_pool, [this]() {
        qDebug() << "prepare(): DBst() m_pool started";
        while (!checkIsRunningThreadPool()) {
//        while (!m_pThrd->isRunning())
//            qDebug() << "waiting for thread";
        }
        qDebug() << "Thread started " << QTime::currentTime();

        return true;
    });
}

QFuture<bool> DBst::open()
{
    QFuture<bool> future;
    qDebug() << "opening db()";
    if (m_isDbFileExist && m_isRunningThreadPool) {
        future = QtConcurrent::run(&m_pool, [this]() {
            qDebug() << m_dbType << m_connName << m_dbName;
            auto m_db = QSqlDatabase::addDatabase(m_dbType, m_connName);
            m_db.setDatabaseName(m_dbName);
            if (m_db.open()) {
                qDebug() << "DB thread opened: " << QThread::currentThreadId();
                return true;
            } else {
                qDebug() << "Open db error: " << m_db.lastError().text();
                return false;
            }
        });
    } else {
        future = QtConcurrent::run(&m_pool, []() {
            return false;
        });
        m_willRunOpen = true;
    }
    return future;
}

QFuture<bool> DBst::execAndCheck(QSqlQuery &query)
{
    return QtConcurrent::run(&m_pool, [query]() {
        QSqlQuery newQ(query);
        return newQ.exec();
    });
}

QFuture<QSqlQuery> DBst::execAndGetQuery(QSqlQuery &query)
{
    return QtConcurrent::run(&m_pool, [query]() {
        QSqlQuery newQ(query);
        newQ.exec();
        return newQ;
    });
}

const QSqlDatabase &DBst::db()
{
    m_db = QSqlDatabase::database(m_connName);
    return m_db;
}

void DBst::setPoolThreadIsRunning()
{
    qDebug() << "setPoolThreadIsRunning";
    m_mutex.lock();
    m_isRunningThreadPool = true;
    m_mutex.unlock();
    if (m_willRunOpen) {
        this->open();
        m_willRunOpen = false;
    }
}

void DBst::initDbSets()
{
    qDebug() << "DBst() " << QThread::currentThreadId();
    QFile dbFile;
    QString fileName;
#ifdef QT_DEBUG
    fileName = "d:/Projects/qt/TVmish/db/TVDB.db3";
#else
    fileName = QGuiApplication::applicationDirPath().toStdString() + "/TVDB.db3";
#endif
    dbFile.setFileName(fileName);
    if (!dbFile.exists()) {
        m_isDbFileExist = false;
        qWarning("DB missing!");
    }
    dbFile.close();

    m_dbName = fileName;
    m_dbType = "QSQLITE";
    m_connName = "connection 1";

    m_pThrd = new PoolThread();
    connect(m_pThrd, SIGNAL(isStarted()), this, SLOT(setPoolThreadIsRunning()));
    m_pool.setMaxThreadCount(1);
    m_pool.setExpiryTimeout(-1);

    m_pool.start(m_pThrd);

    qDebug() << "DBst() m_pool started " << QTime::currentTime();
//    m_db = QSqlDatabase::addDatabase(m_dbType, m_connName);
//    m_db.setDatabaseName(m_dbName);
//    if (m_db.open()) {
////        qDebug() << "DB thread opened: " << this->thread();
//    } else {
//        qDebug() << "Open db error: " << m_db.lastError().text();
    //    }
}

bool DBst::checkIsRunningThreadPool()
{
    bool res;
    m_mutex.lock();
    res = m_isRunningThreadPool;
    m_mutex.unlock();
    return res;
}
