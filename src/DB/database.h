#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql/QSqlDatabase>

class DataBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QSqlDatabase db READ db)
public:
    DataBase(QObject *parent = nullptr);
    ~DataBase();

    bool connect();
    bool disconnect();

    QSqlDatabase db() const;

private:
    QSqlDatabase m_db;

};

#endif // DATABASE_H
