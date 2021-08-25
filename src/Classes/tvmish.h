#ifndef TVMISH_H
#define TVMISH_H

#include <QObject>

#include "DB/database.h"
#include "Controllers/maincontroller.h"

#include <QGuiApplication>

class TVmish : public QObject
{
    Q_OBJECT
public:
    explicit TVmish(QObject *parent = nullptr, QGuiApplication *app = nullptr);
//    TVMish(QGuiApplication app);
    virtual ~TVmish();

//    QString appPath;

    bool startApp();

signals:

private:
    DataBase *db;
    MainController *mainContr;
    std::string m_url;
    std::string m_dbName;
    QGuiApplication *m_app;

};

#endif // TVMISH_H
