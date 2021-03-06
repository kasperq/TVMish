#ifndef TVMISH_H
#define TVMISH_H

#include <QObject>

#include "Controllers/maincontroller.h"
#include "settings.h"

#include "DB/dbst.h"


//#include <QGuiApplication>

class TVmish : public QObject
{
    Q_OBJECT
public:
    TVmish(QString &appPath, QObject *parent);
    virtual ~TVmish();

    bool startApp();

signals:
    void readyForLoadApp();

public slots:
    void loadApp();

private:
    QSqlDatabase m_db;
    QSqlQuery query;

    MainController m_mainContr;
    Settings m_sets;

    std::string m_url;
    std::string m_dbName;
    QString m_appPath;
};

#endif // TVMISH_H
