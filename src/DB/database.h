#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql/QSqlDatabase>

#include <DB/dbconnpool.h>

class DataBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QSqlDatabase db READ db)
public:
    DataBase(QObject *parent = nullptr);
    DataBase(DataBase &value);
    DataBase& operator=(DataBase &orig);
    ~DataBase();

    bool connect();
    bool disconnect();
    bool disconnect(QSqlDatabase &db);

    QSqlDatabase db();

private:
    QSqlDatabase m_db;



//    static DBConnPool m_pool;
};

#endif // DATABASE_H
