if(NOT QT_AVPLAYER_DIR)
    message(FATAL_ERROR "NO QT_AVPLAYER_DIR: ${CMAKE_SOURCE_DIR}")
    set(QT_AVPLAYER_DIR ${CMAKE_SOURCE_DIR})
endif()

option(QT_AVPLAYER_MULTIMEDIA "Enable QtMultimedia" ON)
option(QT_AVPLAYER_VA_X11 "Enable libva-x11" OFF)
option(QT_AVPLAYER_VA_DRM "Enable libva-drm" OFF)
option(QT_AVPLAYER_VDPAU "Enable vdpau" OFF)
option(QT_AVPLAYER_WIDGET_OPENGL "Enable widget opengl" OFF)

if(WIN32)
    # Windows-specific FFmpeg configuration
    set(FFMPEG_ROOT "C:/ffmpeg")
    
    # Include directory
    include_directories(${FFMPEG_ROOT}/include)
    link_directories(${FFMPEG_ROOT}/lib)
    
    # Set FFmpeg libraries (override the find_library results)
    set(AVDEVICE_LIBRARY ${FFMPEG_ROOT}/lib/avdevice.lib)
    set(AVCODEC_LIBRARY ${FFMPEG_ROOT}/lib/avcodec.lib)
    set(AVFILTER_LIBRARY ${FFMPEG_ROOT}/lib/avfilter.lib)
    set(AVFORMAT_LIBRARY ${FFMPEG_ROOT}/lib/avformat.lib)
    set(AVUTIL_LIBRARY ${FFMPEG_ROOT}/lib/avutil.lib)
    set(SWRESAMPLE_LIBRARY ${FFMPEG_ROOT}/lib/swresample.lib)
    set(SWSCALE_LIBRARY ${FFMPEG_ROOT}/lib/swscale.lib)
else()
    # Original FFmpeg library finding for other platforms
    find_library(AVDEVICE_LIBRARY NAMES avdevice)
    find_library(AVCODEC_LIBRARY NAMES avcodec)
    find_library(AVFILTER_LIBRARY NAMES avfilter)
    find_library(AVFORMAT_LIBRARY NAMES avformat)
    find_library(AVUTIL_LIBRARY NAMES avutil)
    find_library(SWRESAMPLE_LIBRARY NAMES swresample)
    find_library(SWSCALE_LIBRARY NAMES swscale)
endif()

set(QtAVPlayer_LIBS
    ${AVDEVICE_LIBRARY}
    ${AVFILTER_LIBRARY}
    ${AVCODEC_LIBRARY}
    ${AVFORMAT_LIBRARY}
    ${AVUTIL_LIBRARY}
    ${SWRESAMPLE_LIBRARY}
    ${SWSCALE_LIBRARY}
)

set(QtAVPlayer_PRIVATE_HEADERS
    ${QT_AVPLAYER_DIR}/qavcodec_p.h
    ${QT_AVPLAYER_DIR}/qavcodec_p_p.h
    ${QT_AVPLAYER_DIR}/qavframecodec_p.h
    ${QT_AVPLAYER_DIR}/qavaudiocodec_p.h
    ${QT_AVPLAYER_DIR}/qavvideocodec_p.h
    ${QT_AVPLAYER_DIR}/qavsubtitlecodec_p.h
    ${QT_AVPLAYER_DIR}/qavhwdevice_p.h
    ${QT_AVPLAYER_DIR}/qavdemuxer_p.h
    ${QT_AVPLAYER_DIR}/qavmuxer_p.h
    ${QT_AVPLAYER_DIR}/qavpacket_p.h
    ${QT_AVPLAYER_DIR}/qavstreamframe_p.h
    ${QT_AVPLAYER_DIR}/qavframe_p.h
    ${QT_AVPLAYER_DIR}/qavpacketqueue_p.h
    ${QT_AVPLAYER_DIR}/qavvideobuffer_p.h
    ${QT_AVPLAYER_DIR}/qavvideobuffer_cpu_p.h
    ${QT_AVPLAYER_DIR}/qavvideobuffer_gpu_p.h
    ${QT_AVPLAYER_DIR}/qavfilter_p.h
    ${QT_AVPLAYER_DIR}/qavfilter_p_p.h
    ${QT_AVPLAYER_DIR}/qavvideofilter_p.h
    ${QT_AVPLAYER_DIR}/qavaudiofilter_p.h
    ${QT_AVPLAYER_DIR}/qavfiltergraph_p.h
    ${QT_AVPLAYER_DIR}/qavinoutfilter_p.h
    ${QT_AVPLAYER_DIR}/qavinoutfilter_p_p.h
    ${QT_AVPLAYER_DIR}/qavvideoinputfilter_p.h
    ${QT_AVPLAYER_DIR}/qavaudioinputfilter_p.h
    ${QT_AVPLAYER_DIR}/qavvideooutputfilter_p.h
    ${QT_AVPLAYER_DIR}/qavaudiooutputfilter_p.h
    ${QT_AVPLAYER_DIR}/qavfilters_p.h
)

set(QtAVPlayer_PUBLIC_HEADERS
    ${QT_AVPLAYER_DIR}/qaviodevice.h
    ${QT_AVPLAYER_DIR}/qavaudioformat.h
    ${QT_AVPLAYER_DIR}/qavstreamframe.h
    ${QT_AVPLAYER_DIR}/qavframe.h
    ${QT_AVPLAYER_DIR}/qavvideoframe.h
    ${QT_AVPLAYER_DIR}/qavaudioframe.h
    ${QT_AVPLAYER_DIR}/qavsubtitleframe.h
    ${QT_AVPLAYER_DIR}/qtavplayerglobal.h
    ${QT_AVPLAYER_DIR}/qavstream.h
    ${QT_AVPLAYER_DIR}/qavplayer.h
    ${QT_AVPLAYER_DIR}/qavaudioconverter.h
)

set(QtAVPlayer_SOURCES
    ${QT_AVPLAYER_DIR}/qavplayer.cpp
    ${QT_AVPLAYER_DIR}/qavcodec.cpp
    ${QT_AVPLAYER_DIR}/qavframecodec.cpp
    ${QT_AVPLAYER_DIR}/qavaudiocodec.cpp
    ${QT_AVPLAYER_DIR}/qavvideocodec.cpp
    ${QT_AVPLAYER_DIR}/qavsubtitlecodec.cpp
    ${QT_AVPLAYER_DIR}/qavdemuxer.cpp
    ${QT_AVPLAYER_DIR}/qavmuxer.cpp
    ${QT_AVPLAYER_DIR}/qavpacket.cpp
    ${QT_AVPLAYER_DIR}/qavframe.cpp
    ${QT_AVPLAYER_DIR}/qavstreamframe.cpp
    ${QT_AVPLAYER_DIR}/qavvideoframe.cpp
    ${QT_AVPLAYER_DIR}/qavaudioframe.cpp
    ${QT_AVPLAYER_DIR}/qavsubtitleframe.cpp
    ${QT_AVPLAYER_DIR}/qavvideobuffer_cpu.cpp
    ${QT_AVPLAYER_DIR}/qavvideobuffer_gpu.cpp
    ${QT_AVPLAYER_DIR}/qavfilter.cpp
    ${QT_AVPLAYER_DIR}/qavvideofilter.cpp
    ${QT_AVPLAYER_DIR}/qavaudiofilter.cpp
    ${QT_AVPLAYER_DIR}/qavfiltergraph.cpp
    ${QT_AVPLAYER_DIR}/qavinoutfilter.cpp
    ${QT_AVPLAYER_DIR}/qavvideoinputfilter.cpp
    ${QT_AVPLAYER_DIR}/qavaudioinputfilter.cpp
    ${QT_AVPLAYER_DIR}/qavvideooutputfilter.cpp
    ${QT_AVPLAYER_DIR}/qavaudiooutputfilter.cpp
    ${QT_AVPLAYER_DIR}/qaviodevice.cpp
    ${QT_AVPLAYER_DIR}/qavstream.cpp
    ${QT_AVPLAYER_DIR}/qavfilters.cpp
    ${QT_AVPLAYER_DIR}/qavaudioconverter.cpp
)

if(WIN32)
    set(QtAVPlayer_PRIVATE_HEADERS
        ${QtAVPlayer_PRIVATE_HEADERS}
        ${QT_AVPLAYER_DIR}/qavhwdevice_d3d11_p.h
    )

    set(QtAVPlayer_SOURCES
        ${QtAVPlayer_SOURCES}
        ${QT_AVPLAYER_DIR}/qavhwdevice_d3d11.cpp
    )
endif()

if(APPLE)
    find_library(IOSURFACE_LIBRARY IOSurface)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    find_library(COREMEDIA_LIBRARY CoreMedia)
    find_library(METAL_LIBRARY Metal)
    find_library(METAL_FOUNDATION Foundation)

    set(QtAVPlayer_SOURCES
        ${QtAVPlayer_SOURCES}
        ${QT_AVPLAYER_DIR}/qavhwdevice_videotoolbox.mm
    )

    set(QtAVPlayer_LIBS
        ${QtAVPlayer_LIBS}
        ${IOSURFACE_LIBRARY}
        ${COREVIDEO_LIBRARY}
        ${COREMEDIA_LIBRARY}
        ${METAL_LIBRARY}
        ${METAL_FOUNDATION}
    )
endif()

if(ANDROID)
    set(QtAVPlayer_SOURCES
        ${QtAVPlayer_SOURCES}
        ${QT_AVPLAYER_DIR}/qavhwdevice_mediacodec.cpp
        ${QT_AVPLAYER_DIR}/qavandroidsurfacetexture.cpp
    )

    # Android-specific FFmpeg configuration
    set(FFMPEG_LIBS_DIR /home/sevizdral/Projects/Drafts/ffmpeg/ffmpegAndroid/${ANDROID_ABI})
    # Android-specific FFmpeg configuration
    # Include directory
    include_directories(${FFMPEG_LIBS_DIR}/include)

    # For debug builds
    # if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        link_directories(${FFMPEG_LIBS_DIR})
    # endif()

    # Set FFmpeg libraries (override the find_library results)
    set(AVDEVICE_LIBRARY ${FFMPEG_LIBS_DIR}/libavdevice.so)
    set(AVCODEC_LIBRARY ${FFMPEG_LIBS_DIR}/libavcodec.so)
    set(AVFILTER_LIBRARY ${FFMPEG_LIBS_DIR}/libavfilter.so)
    set(AVFORMAT_LIBRARY ${FFMPEG_LIBS_DIR}/libavformat.so)
    set(AVUTIL_LIBRARY ${FFMPEG_LIBS_DIR}/libavutil.so)
    set(SWRESAMPLE_LIBRARY ${FFMPEG_LIBS_DIR}/libswresample.so)
    set(SWSCALE_LIBRARY ${FFMPEG_LIBS_DIR}/libswscale.so)

    # Update QtAVPlayer_LIBS with the full paths
    set(QtAVPlayer_LIBS
        ${AVDEVICE_LIBRARY}
        ${AVFILTER_LIBRARY}
        ${AVCODEC_LIBRARY}
        ${AVFORMAT_LIBRARY}
        ${AVUTIL_LIBRARY}
        ${SWRESAMPLE_LIBRARY}
        ${SWSCALE_LIBRARY}
    )
endif()

if(QT_AVPLAYER_MULTIMEDIA)
    message(STATUS "QT_AVPLAYER_MULTIMEDIA is defined")
    add_definitions(-DQT_AVPLAYER_MULTIMEDIA)

    set(QtAVPlayer_PUBLIC_HEADERS
        ${QtAVPlayer_PUBLIC_HEADERS}
        ${QT_AVPLAYER_DIR}/qavaudiooutput.h
        ${QT_AVPLAYER_DIR}/qavaudiooutputdevice.h
    )

    set(QtAVPlayer_SOURCES
        ${QtAVPlayer_SOURCES}
        ${QT_AVPLAYER_DIR}/qavaudiooutput.cpp
        ${QT_AVPLAYER_DIR}/qavaudiooutputdevice.cpp
    )
endif()

if(QT_AVPLAYER_VA_X11)
    message(STATUS "QT_AVPLAYER_VA_X11 is defined")
    add_definitions(-DQT_AVPLAYER_VA_X11)

    set(QtAVPlayer_LIBS
        ${QtAVPlayer_LIBS}
        OpenGL::GL
        X11
        va-x11
        va
    )

    set(QtAVPlayer_PRIVATE_HEADERS
        ${QtAVPlayer_PRIVATE_HEADERS}
        ${QT_AVPLAYER_DIR}/qavhwdevice_vaapi_x11_glx_p.h
    )

    set(QtAVPlayer_SOURCES
        ${QtAVPlayer_SOURCES}
        ${QT_AVPLAYER_DIR}/qavhwdevice_vaapi_x11_glx.cpp
    )
endif()

if(QT_AVPLAYER_VA_DRM)
    message(STATUS "QT_AVPLAYER_VA_DRM is defined")

    # Search for the drm_fourcc.h file in both possible locations
    find_path(DRM_FOURCC_H_PATH drm_fourcc.h
            PATHS
            /usr/include/drm
            /usr/include/libdrm
            NO_DEFAULT_PATH
    )

    # Check if the file was found
    if (DRM_FOURCC_H_PATH)
        message(STATUS "Found drm_fourcc.h at ${DRM_FOURCC_H_PATH}")

        # Check if the file is in both locations and prefer the drm version
        find_path(DRM_PATH drm_fourcc.h PATHS /usr/include/drm NO_DEFAULT_PATH)
        if (DRM_PATH AND DRM_PATH STREQUAL DRM_FOURCC_H_PATH)
            message(STATUS "Using /usr/include/drm version of drm_fourcc.h")
            include_directories(/usr/include/drm)
        else()
            message(STATUS "Using ${DRM_FOURCC_H_PATH} version of drm_fourcc.h")
            include_directories(${DRM_FOURCC_H_PATH})
        endif()
    else()
        message(FATAL_ERROR "drm_fourcc.h not found in the specified paths")
    endif()

    add_definitions(-DQT_AVPLAYER_VA_DRM)

    find_package(OpenGL REQUIRED COMPONENTS OpenGL EGL)
    set(QtAVPlayer_LIBS
        ${QtAVPlayer_LIBS}
        OpenGL::GL
        EGL
        va-drm
        va
    )

    set(QtAVPlayer_PRIVATE_HEADERS
        ${QtAVPlayer_PRIVATE_HEADERS}
        ${QT_AVPLAYER_DIR}/qavhwdevice_vaapi_drm_egl_p.h
    )

    set(QtAVPlayer_SOURCES
        ${QtAVPlayer_SOURCES}
        ${QT_AVPLAYER_DIR}/qavhwdevice_vaapi_drm_egl.cpp
    )
endif()

if(QT_AVPLAYER_VDPAU)
    message(STATUS "QT_AVPLAYER_VDPAU is defined")
    add_definitions(-DQT_AVPLAYER_VDPAU)

    find_package(OpenGL REQUIRED)
    set(QtAVPlayer_LIBS
        ${QtAVPlayer_LIBS}
        OpenGL::GL
    )

    set(QtAVPlayer_PRIVATE_HEADERS
        ${QtAVPlayer_PRIVATE_HEADERS}
        ${QT_AVPLAYER_DIR}/qavhwdevice_vdpau_p.h
    )

    set(QtAVPlayer_SOURCES
        ${QtAVPlayer_SOURCES}
        ${QT_AVPLAYER_DIR}/qavhwdevice_vdpau.cpp
    )
endif()

if(QT_AVPLAYER_WIDGET_OPENGL)
    message(STATUS "QT_AVPLAYER_WIDGET_OPENGL is defined")
    set(QtAVPlayer_PUBLIC_HEADERS
        ${QtAVPlayer_PUBLIC_HEADERS}
        ${QT_AVPLAYER_DIR}/qavwidget_opengl.h
    )

    set(QtAVPlayer_SOURCES
        ${QtAVPlayer_SOURCES}
        ${QT_AVPLAYER_DIR}/qavwidget_opengl.cpp
    )
endif()
