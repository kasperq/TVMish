#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QGuiApplication>

#include "playlistcontroller.h"
#include "tvcontroller.h"

class MainController : public QObject
{
    Q_OBJECT
public:
    MainController();
    MainController& operator=(MainController &orig);
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

    PlaylistController m_plContr {engine};
    TVController m_tvContr {engine};    
    Settings m_sets;
};

#endif // MAINCONTROLLER_H
