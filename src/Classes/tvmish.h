#ifndef TVMISH_H
#define TVMISH_H

#include <QObject>

#include "DB/database.h"
#include "Controllers/maincontroller.h"
#include "settings.h"

//#include <QGuiApplication>

class TVmish : public QObject
{
    Q_OBJECT
public:
    TVmish(QString &appPath);
    virtual ~TVmish();

    bool startApp();

signals:

private:
    DataBase db;
    MainController m_mainContr {db};
    Settings m_sets;

    std::string m_url;
    std::string m_dbName;
    QString m_appPath;


};

#endif // TVMISH_H
