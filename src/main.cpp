#include <iostream>
#include <QtWidgets/QApplication>
#include <QGuiApplication>
#include <QtQuick/QQuickView>
#include <QWindow>

#include "Classes/tvmish.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

     // app.setScreenInhibitPolicy(QGuiApplication::ScreenInhibitPolicy::InhibitAutomaticScreenOff);
//    bool openGLSupported = QQuickWindow::graphicsApi() == QSGRendererInterface::OpenGLRhi;
//    if (!openGLSupported) {
//        qWarning() << "OpenGL is not set as the graphics backend, so AbstractSeries.useOpenGL will not work.";
//        qWarning() << "Set QSG_RHI_BACKEND=opengl environment variable to force the OpenGL backend to be used.";
//    }

// #ifdef Q_OS_WIN
//     QString extraImportPath(QStringLiteral("%1/../../../../%2"));
// #else
//     QString extraImportPath(QStringLiteral("%1/../../../%2"));
// #endif

#ifdef ARCH_X86_64
    setenv("AVPLAYER_ANDROID_LIB_X86_64", "/home/sevizdral/ffmpeg/install-android/lib", 1);
    setenv("CPLUS_INCLUDE_PATH", "/home/sevizdral/ffmpeg/install-android/include", 1);
#endif
#ifdef ARCH_v7a
    setenv("AVPLAYER_ANDROID_LIB_ARMEABI_V7A", "/home/sevizdral/ffmpeg/install-android_v7/lib", 1);
    setenv("CPLUS_INCLUDE_PATH", "/home/sevizdral/ffmpeg/install-android_v7/include", 1);
#endif
#ifdef ARCH_v8
    setenv("AVPLAYER_ANDROID_LIB_ARMEABI_V8A", "/home/sevizdral/ffmpeg/install-android_v8/lib", 1);
    setenv("CPLUS_INCLUDE_PATH", "/home/sevizdral/ffmpeg/install-android_v8/include", 1);
#endif

    QString patternDebug = QStringLiteral("[%{time hh:mm:ss.zzz}] [%{threadid}] %{if-debug}[DEBG]%{endif}%{if-warning}[WARN]%{endif}%{if-info}[INFO]%{endif}%{if-critical}[CRIT]%{endif}"
                                          "\t%{if-debug}\033[34m%{endif}\%{if-warning}\033[33m%{endif}%{if-info}\033[32m%{endif} %{if-critical}\033[31m%{endif}%{function}[%{line}]\033[0m: %{message}");
    qSetMessagePattern(patternDebug);

    QString appPath = app.applicationDirPath();

    TVmish* tv = new TVmish(appPath, &app);
    if (tv->startApp()){
        return app.exec();
    }
    else
        return 0;

}
