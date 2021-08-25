QT += qml \
      quick \
      sql \
      widgets
TARGET = TVmish
TEMPLATE = app

CONFIG += c++11
#CONFIG -= app_bundle

win32 {
    INCLUDEPATH += c:/boost
#    LIBS += c:/boost/stage/lib/libboost_date_time-mgw46-1_48.a
}

SOURCES += \
        ./Classes/tvmish.cpp \
        Controllers/maincontroller.cpp \
        DB/database.cpp \
        main.cpp

HEADERS += \
    ./Classes/tvmish.h \
    Controllers/maincontroller.h \
    DB/database.h

DISTFILES += \
    Forms/MainForm.qml

RESOURCES += \
    resources.qrc


