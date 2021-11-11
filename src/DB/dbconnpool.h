#ifndef DBCONNPOOL_H
#define DBCONNPOOL_H

#include <QtSql>
#include <QQueue>
#include <QString>
#include <QMutex>
#include <QMutexLocker>
#include <QAtomicPointer>

class DBConnPool
{
public:
    static void closeAll();
    static QSqlDatabase openConnection();
    static void closeConnection(QSqlDatabase connection);
    ~DBConnPool();
    static DBConnPool &getInstance();

    QSqlDatabase createConnection(const QString &connName);
    QQueue<QString> usedConnNames;
    QQueue<QString> unusedConnNames;


private:
    DBConnPool();
    DBConnPool(const DBConnPool &otherPool);
    DBConnPool& operator=(const DBConnPool &otherPool);

    static QMutex m_mutex;
    static QWaitCondition m_waitCondition;
    static QAtomicPointer<DBConnPool> m_instance;

    QString m_host;
    QString m_dbName;
    QString m_userName;
    QString m_password;
    QString m_dbType;
    int m_port;
    bool m_testOnBorrow;
    QString m_testSql;
    int m_maxWaitTime;
    int m_waitInterval;
    int m_maxConnectionCount;

};

#endif // DBCONNPOOL_H
