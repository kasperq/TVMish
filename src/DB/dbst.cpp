#include "dbst.h"

#include <QFile>
#include <QDebug>
#include <QSqlError>

//#include <QtConcurrent>
//#include <QFuture>
#include <variant>


#include <QTime>

QMutex DBst::m_mutex;
QAtomicPointer<DBst> DBst::m_instance = nullptr;
QWaitCondition DBst::m_waitCondition;

DBst &DBst::getInstance()
{
#ifndef Q_ATOMIC_POINTER_TEST_AND_SET_IS_ALWAYS_NATIVE
    if (!QAtomicPointer<DBst>::isTestAndSetNative())
        qDebug() << "Error: TestAndSetNative not supported!";
#endif
    {
        QMutexLocker locker(&m_mutex);
        if (m_instance.testAndSetOrdered(nullptr, nullptr)) {
            m_instance.testAndSetOrdered(nullptr, new DBst);
        }
        return *m_instance.loadAcquire();
    }
}

DBst::DBst(QObject *parent) : QObject(parent)
{
    initDbSets();
}

DBst::DBst()
{
    initDbSets();
}

DBst::DBst(const DBst &value, QObject *parent) : QObject(parent)
{
    *this = value;    
}

DBst &DBst::operator=(const DBst &value)
{
    *this = value;
    return *this;
}

bool DBst::isDbThrOpened() const
{
    return m_isDbThrOpened;
}

void DBst::setIsDbThrOpened(const bool &value)
{
    qDebug() << "setIsDbThrOpened: " << value;
    m_isDbThrOpened = value;
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
            m_db.setConnectOptions("QSQLITE_OPEN_URI;QSQLITE_BUSY_TIMEOUT=5000;QSQLITE_ENABLE_SHARED_CACHE;SQLITE_OPEN_MEMORY");
            bool res {false};
            if (m_db.open()) {
                res = true;
                qDebug() << "DB thread opened: " << QThread::currentThreadId();
                emit changeIsDbOpened(res);
                return true;
            } else {
                res = false;
                qDebug() << "Open db error: " << m_db.lastError().text();
                emit changeIsDbOpened(res);
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

QFuture<bool> DBst::close()
{
    return QtConcurrent::run(&m_pool, [this]() {        
        QSqlDatabase::database(m_connName).close();        

        return true;
    });
}

void DBst::closeInstance()
{
    QMutexLocker locker(&m_mutex);
    delete m_instance;
    m_instance = nullptr;
}

QFuture<bool> DBst::execAndCheck(QSqlQuery &query)
{
    return QtConcurrent::run(&m_pool, [this, query]() {
        if (!QSqlDatabase::database(m_connName).isOpen()) {
            qDebug() << "db is closed";
            QSqlDatabase::database(m_connName).open();
        }
        QSqlQuery newQ(QSqlDatabase::database(m_connName));        
        newQ = query;

        qDebug() << "Dbst: execAndCheck 3";
        try {
            if (newQ.exec())
                return true;
            else {
                qDebug() << "execAndCheck error: " << newQ.lastError();
                return false;
            }
        }  catch (...) {
            qDebug() << "execAndCheck error: " << newQ.lastError();
        }
        return false;
    });
}

QFuture<QSqlQuery> DBst::execAndGetQuery(QSqlQuery &query)
{
    return QtConcurrent::run(&m_pool, [this, query]() {
        QSqlQuery newQ(QSqlDatabase::database(m_connName));
//        newQ = QSqlQuery((QSqlDatabase::database(m_connName)));
        newQ = query;        
        newQ.exec();
        return newQ;
    });
}

const QSqlDatabase &DBst::db()
{
    m_db1 = QSqlDatabase::database(m_connName);
    return m_db1;
}

const QSqlDatabase &DBst::db_def()
{
    return m_dbDef;
}

QFuture<bool> DBst::commitDB()
{
    m_isTransStarted = false;
    return QtConcurrent::run(&m_pool, [this]() {
//        return QSqlDatabase::database(m_connName).commit();
        return QSqlDatabase::database(m_connName).driver()->commitTransaction();
    });
}

QFuture<bool> DBst::startTransDB()
{
    if (m_isTransStarted) {
        QFuture<bool> res = QtConcurrent::run(&m_pool, []() { return true; });
        return res;
    }
    m_isTransStarted = true;
    return QtConcurrent::run(&m_pool, [this]() {
        return QSqlDatabase::database(m_connName).driver()->beginTransaction();
        //       return QSqlDatabase::database(m_connName).transaction();
    });
}

bool DBst::commitDbDef()
{
    qDebug() << "DBst::commitDbDef()";
//    return QSqlDatabase::database(m_connName).driver()->commitTransaction();

//    m_isTransDefStarted = false;
//    m_isTransStarted = false;
    return m_dbDef.commit();
}

bool DBst::startTransDBdef()
{
    if (m_isTransStarted || m_isTransDefStarted) {
        return true;
    }
    m_isTransDefStarted = true;
    m_isTransStarted = true;
//    QFuture<bool> res = QtConcurrent::run(&m_pool, [this]() {
//        return QSqlDatabase::database(m_connName).driver()->beginTransaction();
//    });
//    return res.result();
//    return QSqlDatabase::database(m_connName).driver()->beginTransaction();

    return m_dbDef.driver()->beginTransaction();
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
    connect(this, SIGNAL(changeIsDbOpened(bool)), this, SLOT(setIsDbThrOpened(bool)));
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

    initDefDb();

    qDebug() << "DBst() m_pool started " << QTime::currentTime();
//    m_db = QSqlDatabase::addDatabase(m_dbType, m_connName);
//    m_db.setDatabaseName(m_dbName);
//    if (m_db.open()) {
////        qDebug() << "DB thread opened: " << this->thread();
//    } else {
//        qDebug() << "Open db error: " << m_db.lastError().text();
    //    }
}

void DBst::initDefDb()
{
    m_mainThrConnName = "default_connection";
    m_dbDef = QSqlDatabase::addDatabase(m_dbType, m_mainThrConnName);
    m_dbDef.setDatabaseName(m_dbName);    
    qDebug() << "hasTrans: " << m_dbDef.driver()->hasFeature(QSqlDriver::Transactions);
    m_dbDef.setConnectOptions("QSQLITE_OPEN_URI;QSQLITE_BUSY_TIMEOUT=5000;QSQLITE_ENABLE_SHARED_CACHE;SQLITE_OPEN_MEMORY");
    if (m_dbDef.open()) {                
        qDebug() << "DBdef opened: " << QThread::currentThreadId() << " trans: " << m_dbDef.transaction();
    } else {
        qDebug() << "Open dbdef error: " << m_dbDef.lastError().text();
    }
}

bool DBst::checkIsRunningThreadPool()
{
    bool res;
    m_mutex.lock();
    res = m_isRunningThreadPool;
    m_mutex.unlock();
    return res;
}
