#ifndef SETSTHREAD_H
#define SETSTHREAD_H

#include <QThread>
#include <QObject>
#include <QDebug>

//#include "DB/database.h"
#include <QSqlQuery>
#include <QSqlDatabase>

class SetsThread : public QThread
{
    Q_OBJECT
public:
    SetsThread();
//    SetsThread(DataBase &db);
    SetsThread(QSqlDatabase &qdb);

//    void setDb(QSqlDatabase &qdb);
public slots:
    void modify(QString &appPath, QString &curPlDir);

private:
//    std::shared_ptr< DataBase > m_db;
    std::shared_ptr< QSqlDatabase > m_qdb;

    QSqlQuery q_temp;

    void run() override;
};

#endif // SETSTHREAD_H
