#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QGuiApplication>

class MainController : public QObject
{
    Q_OBJECT
public:
    explicit MainController(QObject *parent = nullptr, QGuiApplication *app = nullptr);
    virtual ~MainController();

    void loadMainForm();


signals:

private:
    QQmlApplicationEngine engine;
    QGuiApplication *m_app;

};

#endif // MAINCONTROLLER_H
