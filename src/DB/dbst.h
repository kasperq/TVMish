#ifndef DBST_H
#define DBST_H

#include <QThread>
#include <QThreadPool>
#include <QFuture>
#include <QtConcurrent>
#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>

#include <QSqlDriver>

#include "Thread/poolthread.h"

class DBst : public QObject
{
    Q_OBJECT    
public:
    static DBst &getInstance();
    DBst(QObject *parent);
    DBst();
//    DBst(const DBst& value, QObject *parent);
//    DBst& operator=(const DBst& value);
    virtual ~DBst();

    QFuture<bool> isPrepared();
    QFuture<bool> open();
    QFuture<bool> close();
    static void closeInstance();

    QFuture<bool> execAndCheck(QSqlQuery &query);
    QFuture<QSqlQuery> execAndGetQuery(QSqlQuery &query);

    const QSqlDatabase &db();
    const QSqlDatabase &db_def();
    QFuture<bool> commitDB();
    QFuture<bool> startTransDB();
    bool commitDbDef();
    bool startTransDBdef();

    bool isDbThrOpened() const;

signals:
    void changeIsDbOpened(const bool &value);
public slots:
    void setPoolThreadIsRunning();
    void setIsDbThrOpened(const bool &newIsDbThrOpened);

private:
    DBst(const DBst& value, QObject *parent);
    DBst& operator=(const DBst& value);

    static QMutex m_mutex;
    static QWaitCondition m_waitCondition;
    static QAtomicPointer<DBst> m_instance;

    QThreadPool m_pool;
    PoolThread *m_pThrd;

    QSqlDatabase m_db1;
    QSqlQuery m_query;

//    QSqlQuery newQ;

    QSqlDatabase m_dbDef;

    bool m_isDbFileExist {true};
    bool m_isDbThrOpened {false};
    QString m_dbName {};
    QString m_dbType {};
    QString m_connName {};
    QString m_mainThrConnName {};

    bool m_isRunningThreadPool {false};
    bool m_willRunOpen {false};
    bool m_isTransStarted {false};
    bool m_isTransDefStarted {false};

    void initDbSets();
    void initDefDb();
    bool checkIsRunningThreadPool();
};

#endif // DBST_H
