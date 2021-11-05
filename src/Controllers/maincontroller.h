#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QGuiApplication>

#include "playlistcontroller.h"

class MainController : public QObject
{
    Q_OBJECT
public:
    MainController(QSqlDatabase db);
    virtual ~MainController();

    void loadMainForm();


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
    PlaylistController *plContr;



};

#endif // MAINCONTROLLER_H
