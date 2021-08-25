#include <QtQml/QQmlEngine>


#include "maincontroller.h"

MainController::MainController(QObject *parent, QGuiApplication *app) : QObject(parent), m_app(app)
{

}

MainController::~MainController()
{
//    delete viewer;
}

void MainController::loadMainForm()
{
    engine.load(QUrl(QStringLiteral("qrc:/Forms/MainForm.qml")));
    if (engine.rootObjects().isEmpty())
        qDebug() << "exit";

//    const QUrl url(QStringLiteral("qrc:/Forms/MainForm.qml"));
//    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
//                     m_app, [url](QObject *obj, const QUrl &objUrl) {
//        if (!obj && url == objUrl) {
//            qDebug() << "exit app";
//            QCoreApplication::exit(-1);
//        }
//        }, Qt::QueuedConnection);
//    engine.load(url);

    //    viewer = new QQuickView();
////    connect(viewer->engine(), &QQmlEngine::quit, &viewer, &QWindow::close);
//    viewer->setTitle(QStringLiteral("TVmish"));

////    viewer->rootContext()->setContextProperty("dataSource", &dataSource);
////    viewer->rootContext()->setContextProperty("openGLSupported", openGLSupported);

//    viewer->setSource(QUrl("qrc:/Forms/MainForm.qml"));
//    viewer->setResizeMode(QQuickView::SizeRootObjectToView);
////    viewer->setColor(QColor("#404040"));
//    viewer->show();
}
