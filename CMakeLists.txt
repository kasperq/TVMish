cmake_minimum_required(VERSION 3.8)
project(TVmish LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.8 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Sql
    Concurrent
    Multimedia
    Quick
    Qml
    QuickControls2
)

qt_standard_project_setup(REQUIRES 6.8)

# Android-specific configuration
if(ANDROID)
    # Set Android-specific properties
    set(CMAKE_ANDROID_NDK $ENV{ANDROID_NDK_ROOT})
    set(CMAKE_ANDROID_STL_TYPE c++_shared)

    # Force Release build for Android if not specified
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release)
    endif()

    # Set output directories
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs/${ANDROID_ABI})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs/${ANDROID_ABI})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs/${ANDROID_ABI})

    # This tells CMake we're building an APK
    set(CMAKE_ANDROID_ANT_ADDITIONAL_OPTIONS "-Dqt.deployment=true")
else()
    set(CMAKE_CURRENT_SOURCE_DIR ${CMAKE_SOURCE_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()


# Set default build type to Release if not specified
# if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
#     set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build" FORCE)
# endif()

if(Qt6CoreTools_FOUND)
    find_package(Qt6 COMPONENTS QmlTools Quick REQUIRED)
    qt6_add_resources(QT_RESOURCES src/resources.qrc)
    find_package(Qt6 COMPONENTS MultimediaQuickPrivate REQUIRED)
    include_directories(${Qt6MultimediaQuick_PRIVATE_INCLUDE_DIRS})
    include_directories(${Qt6Multimedia_PRIVATE_INCLUDE_DIRS})
    find_package(Qt6 REQUIRED COMPONENTS Core BuildInternals OPTIONAL_COMPONENTS Multimedia)
endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)
set(QT_AVPLAYER_DIR ./src/QtAVPlayer/)
include(./src/QtAVPlayer/QtAVPlayer.cmake)

set(SOURCES
    ${QtAVPlayer_SOURCES}
    src/resources.qrc
    src/main.cpp
    src/Classes/tvmish.h src/Classes/tvmish.cpp
    src/Classes/additional.h
    src/Classes/categories.h src/Classes/categories.cpp
    src/Classes/category.h src/Classes/category.cpp
    src/Classes/channel.h src/Classes/channel.cpp
    src/Classes/channels.h src/Classes/channels.cpp
    src/Classes/epg.h src/Classes/epg.cpp
    src/Classes/epgkart.h src/Classes/epgkart.cpp
    src/Classes/epgs.h src/Classes/epgs.cpp
    src/Classes/epgschedule.h src/Classes/epgschedule.cpp
    src/Classes/filecopier.h src/Classes/filecopier.cpp
    src/Classes/filedownloader.h src/Classes/filedownloader.cpp
    src/Classes/logos.h src/Classes/logos.cpp
    src/Classes/playlist.h src/Classes/playlist.cpp
    src/Classes/playlists.h src/Classes/playlists.cpp
    src/Classes/plfile.h src/Classes/plfile.cpp
    src/Classes/plfiles.h src/Classes/plfiles.cpp
    src/Classes/settings.h src/Classes/settings.cpp
    src/Classes/videoplayer.h src/Classes/videoplayer.cpp
    src/Classes/videoitem.h src/Classes/videoitem.cpp
    src/Controllers/epgcontroller.h src/Controllers/epgcontroller.cpp
    src/Controllers/maincontroller.h src/Controllers/maincontroller.cpp
    src/Controllers/playlistcontroller.h src/Controllers/playlistcontroller.cpp
    src/Controllers/tvcontroller.h src/Controllers/tvcontroller.cpp
    src/DB/database.h src/DB/database.cpp
    src/DB/dbconnpool.h src/DB/dbconnpool.cpp
    src/DB/dbst.h src/DB/dbst.cpp
    src/Gateways/categorygw.h src/Gateways/categorygw.cpp
    src/Gateways/channelgw.h src/Gateways/channelgw.cpp
    src/Gateways/epggw.h src/Gateways/epggw.cpp
    src/Gateways/logosgw.h src/Gateways/logosgw.cpp
    src/Gateways/playlistgw.h src/Gateways/playlistgw.cpp
    src/Gateways/plfilegw.h src/Gateways/plfilegw.cpp
    src/Models/categorymodel.h src/Models/categorymodel.cpp
    src/Models/channelsmodel.h src/Models/channelsmodel.cpp
    src/Models/epgmodel.h src/Models/epgmodel.cpp
    src/Models/playlistsmodel.h src/Models/playlistsmodel.cpp
    src/Models/plfilemodel.h src/Models/plfilemodel.cpp
    src/Thread/filecopythread.h src/Thread/filecopythread.cpp
    src/Thread/playlistgwthread.h src/Thread/playlistgwthread.cpp
    src/Thread/playlistparsethread.h src/Thread/playlistparsethread.cpp
    src/Thread/poolthread.h src/Thread/poolthread.cpp
    src/Thread/setsthread.h src/Thread/setsthread.cpp
)

# Android builds should create a shared library
# if(ANDROID)
#     # Add Android manifest and package settings
#     set(ANDROID_MANIFEST ${CMAKE_CURRENT_SOURCE_DIR}/android/AndroidManifest.xml)
#     if(NOT EXISTS ${ANDROID_MANIFEST})
#         message(FATAL_ERROR "AndroidManifest.xml not found at ${ANDROID_MANIFEST}")
#     endif()

#     add_library(${PROJECT_NAME} SHARED ${SOURCES} ${QT_RESOURCES})
#     set_target_properties(${PROJECT_NAME} PROPERTIES
#         OUTPUT_NAME "lib${PROJECT_NAME}"
#         ANDROID_PACKAGE_NAME "com.yourcompany.${PROJECT_NAME}"
#         ANDROID_ABI ${ANDROID_ABI}
#         ANDROID_NDK ${CMAKE_ANDROID_NDK}
#     )
# else()
#     add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${QT_RESOURCES})
# endif()

set(LIBS ${QtAVPlayer_LIBS})

qt_add_executable(TVmish ${SOURCES} ${QT_RESOURCES})

# Link libraries
target_link_libraries(TVmish PRIVATE
    ${QtAVPlayer_LIBS}
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
    Qt6::Concurrent
    Qt6::Multimedia
    Qt6::Quick
    Qt6::Qml
    Qt6::QuickControls2
    Qt6::MultimediaQuickPrivate
)

# target_link_libraries(${PROJECT_NAME} ${LIBS} Qt6::Core Qt6::Gui Qt6::Sql Qt6::Concurrent Qt6::Widgets Qt6::Multimedia Qt6::Quick Qt6::MultimediaQuickPrivate)
include(GNUInstallDirs)
install(TARGETS TVmish
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
