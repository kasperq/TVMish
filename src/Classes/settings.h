#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QString>
#include <QSqlQuery>
//#include <QSqlDatabase>
//#include <QThread>

//#include "./Thread/setsthread.h"

class Settings : public QObject
{
    Q_OBJECT
public:
    Settings();
    Settings(const Settings &value, QObject *parent = nullptr);
    virtual ~Settings();

    void getSets();
    QString appPath();
    void setAppPath(const QString &value);

    QString curPlDir();
    void setCurPlDir(const QString &value);



    QString &logoPath();

private:
    QString m_appPath {};
    uint m_rows {};
    QString m_curPlDir {};
    int m_id {};
    QString m_logoPath {"/data/channels_logos/"};

    QSqlQuery q_select;
    QSqlQuery q_temp;

    void countRows();

    void modifySets();
    void insertSets();

    void getSetsContinuation();


};

#endif // SETTINGS_H
