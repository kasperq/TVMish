QT += qml \
      quick \
      sql \
      widgets
TARGET = TVmish
TEMPLATE = app

CONFIG += c++17
#CONFIG -= app_bundle

win32 {
    INCLUDEPATH += c:/boost
#    LIBS += c:/boost/stage/lib/libboost_date_time-mgw46-1_48.a
}

SOURCES += \
        ./Classes/tvmish.cpp \
        Classes/filecopier.cpp \
        Classes/playlist.cpp \
        Classes/playlists.cpp \
        Classes/plfile.cpp \
        Classes/plfiles.cpp \
        Classes/settings.cpp \
        Controllers/maincontroller.cpp \
        Controllers/playlistcontroller.cpp \
        DB/database.cpp \
        Gateways/playlistgw.cpp \
        Gateways/plfilegw.cpp \
        Models/playlistsmodel.cpp \
        Models/plfilemodel.cpp \
        Thread/playlistthread.cpp \
        main.cpp

HEADERS += \
    ./Classes/tvmish.h \
    Classes/filecopier.h \
    Classes/playlist.h \
    Classes/playlists.h \
    Classes/plfile.h \
    Classes/plfiles.h \
    Classes/settings.h \
    Controllers/maincontroller.h \
    Controllers/playlistcontroller.h \
    DB/database.h \
    Gateways/playlistgw.h \
    Gateways/plfilegw.h \
    Models/playlistsmodel.h \
    Models/plfilemodel.h \
    Thread/playlistthread.h

DISTFILES += \
    Qml/MainForm.qml

RESOURCES += \
    resources.qrc


