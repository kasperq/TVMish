#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QSqlQuery>

class Settings
{
public:
    Settings();
    virtual ~Settings();

    QString appPath();
    void setAppPath(const QString *appPath);

    QString curPlDir();
    void setCurPlDir(const QString &curPlDir);

private:
    QString m_appPath;
    uint m_rows;
    QString m_curPlDir;

    QSqlQuery *q_select;
    QSqlQuery *q_temp;

    void countRows();
    void getSets();
    void modifySets();
    void insertSets();


};

#endif // SETTINGS_H
