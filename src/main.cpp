#include <iostream>
//#include <QtWidgets/QApplication>
#include <QGuiApplication>
#include <QtQuick/QQuickView>

#include "Classes/tvmish.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

//    bool openGLSupported = QQuickWindow::graphicsApi() == QSGRendererInterface::OpenGLRhi;
//    if (!openGLSupported) {
//        qWarning() << "OpenGL is not set as the graphics backend, so AbstractSeries.useOpenGL will not work.";
//        qWarning() << "Set QSG_RHI_BACKEND=opengl environment variable to force the OpenGL backend to be used.";
//    }

#ifdef Q_OS_WIN
    QString extraImportPath(QStringLiteral("%1/../../../../%2"));
#else
    QString extraImportPath(QStringLiteral("%1/../../../%2"));
#endif
    QString appPath = app.applicationDirPath();

    TVmish* tv = new TVmish(&appPath);
    if (tv->startApp()){
        return app.exec();
    }
    else
        return 0;

}
