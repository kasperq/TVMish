QT += concurrent \
        qml \
      quick \      
      sql \
      widgets \      
      core \
      gui-private \
      multimedia
TARGET = TVmish
TEMPLATE = app

CONFIG += c++17
#CONFIG -= app_bundle

DEFINES += "QT_AVPLAYER_MULTIMEDIA"

include(QtAVPlayer/QtAVPlayer.pri)

lessThan(QT_MAJOR_VERSION, 6): QT += qtmultimediaquicktools-private
equals(QT_MAJOR_VERSION, 6): QT += multimediaquick-private

SOURCES += \
        ./Classes/tvmish.cpp \
        Classes/categories.cpp \
        Classes/category.cpp \
        Classes/channel.cpp \
        Classes/channels.cpp \
        Classes/epg.cpp \
        Classes/epgkart.cpp \
        Classes/epgs.cpp \
        Classes/epgschedule.cpp \
        Classes/filecopier.cpp \
        Classes/filedownloader.cpp \
        Classes/logos.cpp \
        Classes/playlist.cpp \
        Classes/playlists.cpp \
        Classes/plfile.cpp \
        Classes/plfiles.cpp \
        Classes/settings.cpp \
        Classes/videoplayer.cpp \
        Classes/videoitem.cpp \
        Controllers/epgcontroller.cpp \
        Controllers/maincontroller.cpp \
        Controllers/playlistcontroller.cpp \
        Controllers/tvcontroller.cpp \
        DB/database.cpp \
        DB/dbconnpool.cpp \
        DB/dbst.cpp \
        Gateways/categorygw.cpp \
        Gateways/channelgw.cpp \
        Gateways/epggw.cpp \
        Gateways/logosgw.cpp \
        Gateways/playlistgw.cpp \
        Gateways/plfilegw.cpp \
        Models/categorymodel.cpp \
        Models/channelsmodel.cpp \
        Models/epgmodel.cpp \
        Models/playlistsmodel.cpp \
        Models/plfilemodel.cpp \
        Thread/filecopythread.cpp \
        Thread/playlistgwthread.cpp \
        Thread/playlistparsethread.cpp \
        Thread/poolthread.cpp \
        Thread/setsthread.cpp \
        main.cpp

HEADERS += \
    ./Classes/tvmish.h \
    Classes/additional.h \
    Classes/categories.h \
    Classes/category.h \
    Classes/channel.h \
    Classes/channels.h \
    Classes/epg.h \
    Classes/epgkart.h \
    Classes/epgs.h \
    Classes/epgschedule.h \
    Classes/filecopier.h \
    Classes/filedownloader.h \
    Classes/logos.h \
    Classes/playlist.h \
    Classes/playlists.h \
    Classes/plfile.h \
    Classes/plfiles.h \
    Classes/settings.h \
    Classes/videoplayer.h \
    Classes/videoitem.h \
    Controllers/epgcontroller.h \
    Controllers/maincontroller.h \
    Controllers/playlistcontroller.h \
    Controllers/tvcontroller.h \
    DB/database.h \
    DB/dbconnpool.h \
    DB/dbst.h \
    Gateways/categorygw.h \
    Gateways/channelgw.h \
    Gateways/epggw.h \
    Gateways/logosgw.h \
    Gateways/playlistgw.h \
    Gateways/plfilegw.h \
    Models/categorymodel.h \
    Models/channelsmodel.h \
    Models/epgmodel.h \
    Models/playlistsmodel.h \
    Models/plfilemodel.h \
    Thread/filecopythread.h \
    Thread/playlistgwthread.h \
    Thread/playlistparsethread.h \
    Thread/poolthread.h \
    Thread/setsthread.h

DISTFILES += \
    Qml/MainForm.qml

RESOURCES += \
    resources.qrc


android: include(/home/sevizdral/Android/Sdk/android_openssl/openssl.pri)
