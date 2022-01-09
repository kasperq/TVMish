#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QString>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QThread>

#include "./Thread/setsthread.h"

class Settings : public QObject
{
    Q_OBJECT
public:
    Settings();
    Settings(const Settings &value);
    Settings(const QSqlDatabase &value);
    virtual ~Settings();

    QString appPath();
    void setAppPath(const QString &value);

    QString curPlDir();
    void setCurPlDir(const QString &value);

    void setDB(const QSqlDatabase &db);

    void modifyThr(QString &appPath, QString &curPlDir);

private:
    QString m_appPath;
    uint m_rows;
    QString m_curPlDir;
    int m_id;

    QSqlDatabase m_db;
    QSqlQuery q_select;
    QSqlQuery q_temp;

//    SetsThread m_setsThr {m_db};

    void countRows();
    void getSets();
    void modifySets();
    void insertSets();




};

#endif // SETTINGS_H
