#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QGuiApplication>

#include "playlistcontroller.h"
#include "DB/database.h"

class MainController : public QObject
{
    Q_OBJECT
public:
//    MainController();
    MainController(DataBase &datab);
    MainController(QSqlDatabase db);
    MainController& operator=(MainController &orig);
    virtual ~MainController();

    void loadMainForm();
//    void setDb(DataBase &db);


signals:

public slots:
    void openTvMode();
    void openOptions();
    void openPlaylistManager();
    void openTvSchedule();
    void openPlaylistChooser();

private:
    QQmlApplicationEngine engine;

    QSqlDatabase m_db;
    std::shared_ptr< DataBase > m_dataB;
    DataBase m_datB;
//    PlaylistController plContr {engine, *m_dataB};
    PlaylistController plContr {engine, m_datB};
};

#endif // MAINCONTROLLER_H
