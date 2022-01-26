#ifndef DBST_H
#define DBST_H

#include <QThread>
#include <QThreadPool>
#include <QFuture>
#include <QtConcurrent>
#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>

#include "Thread/poolthread.h"

class DBst : public QObject
{
    Q_OBJECT
public:
    DBst(QObject *parent);
    DBst();
//    DBst(const DBst& value);
//    DBst& operator=(const DBst& value);
    virtual ~DBst();

    QFuture<bool> isPrepared();
    QFuture<bool> open();

    QFuture<bool> execAndCheck(QSqlQuery &query);
    QFuture<QSqlQuery> execAndGetQuery(QSqlQuery &query);

    const QSqlDatabase &db();

public slots:
    void setPoolThreadIsRunning();

private:
    QThreadPool m_pool;
    PoolThread *m_pThrd;

    QSqlDatabase m_db;
    QSqlQuery m_query;

    bool m_isDbFileExist {true};
    QString m_dbName;
    QString m_dbType;
    QString m_connName;

    bool m_isRunningThreadPool {false};
    bool m_willRunOpen {false};

    QMutex m_mutex;

    void initDbSets();
    bool checkIsRunningThreadPool();
};

#endif // DBST_H
