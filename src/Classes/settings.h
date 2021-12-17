#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QSqlQuery>
#include <QSqlDatabase>

class Settings
{
public:
    Settings();
    Settings(const QSqlDatabase &value);
    virtual ~Settings();

    QString appPath();
    void setAppPath(const QString &value);

    QString curPlDir();
    void setCurPlDir(const QString &value);

    void setDB(const QSqlDatabase &db);

private:
    QString m_appPath;
    uint m_rows;
    QString m_curPlDir;

    QSqlDatabase m_db;
    QSqlQuery q_select;
    QSqlQuery q_temp;

    void countRows();
    void getSets();
    void modifySets();
    void insertSets();


};

#endif // SETTINGS_H
