#include "videoitem.h"

#include "QtAVPlayer/qavcodec_p.h"

extern "C" {
#include "libavformat/avformat.h"
}

VideoItem::VideoItem(QQuickItem *parent)
    : QQuickItem(parent)
{
    m_avPlayer = std::make_unique<QAVPlayer>(this);
    m_audioOutput = std::make_unique<QAVAudioOutput>(this);

    // m_videoOut = std::make_shared<VideoOutput>(findChild<VideoOutput*>(QString::fromLatin1("videoOut")));
    // m_videoSink = std::make_shared<QVideoSink>(m_videoOut->videoSink());

    // m_videoOut = findChild<VideoOutput*>(QString::fromLatin1("videoOut"));
    // m_videoSink = m_videoOut->videoSink();

    connect(m_avPlayer.get(), &QAVPlayer::stateChanged, this, &VideoItem::processAvPlaybackState);
    connect(m_avPlayer.get(), &QAVPlayer::mediaStatusChanged, this, &VideoItem::processAvMediaStatus);
    connect(m_avPlayer.get(), &QAVPlayer::errorOccurred, this, &VideoItem::processAvErrorStatus);
    QObject::connect(m_avPlayer.get(), &QAVPlayer::videoFrame, this, &VideoItem::getVideoFrame, Qt::DirectConnection);
    connect(m_avPlayer.get(), &QAVPlayer::audioFrame, this, &VideoItem::getAudioFrame);
}

VideoItem::~VideoItem()
{

}

QString VideoItem::source() const
{
    return m_source;
}

void VideoItem::setSource(const QString &newSource)
{
    if (m_source == newSource)
        return;
    m_source = newSource;
    emit sourceChanged();

    if (m_videoOut == nullptr) {
        m_videoOut = findChild<VideoOutput*>(QString::fromLatin1("videoOut"));
        // m_videoOut = std::make_shared<VideoOutput>(findChild<VideoOutput*>(QString::fromLatin1("videoOut")));

        if (m_videoOut == nullptr) {
            qDebug() << "MediaPlayer NOT found";
            return;
        } else {
            qDebug() << "MediaPlayer FOUND";

            if (m_videoSink == nullptr)
                m_videoSink = m_videoOut->videoSink();
                // m_videoSink = std::make_shared<QVideoSink>(m_videoOut->videoSink());
            setAvPlayerOptions();

            // m_avPlayer->setSource(m_source);
            // m_avPlayer->play();
        }
    }
    m_avPlayer->setSource(m_source);
    m_avPlayer->play();
    setIsPlaying(true);
}

bool VideoItem::isPlaying() const
{
    return m_isPlaying;
}

void VideoItem::setIsPlaying(bool newIsPlaying)
{
    if (m_isPlaying == newIsPlaying)
        return;
    m_isPlaying = newIsPlaying;
    emit isPlayingChanged();
}

void VideoItem::getVideoFrame(const QAVVideoFrame &frame)
{
    // qDebug() << "frame: " << frame.size();
    QVideoFrame videoFrame;
    videoFrame = frame;

    // videoFrame = frame.convertTo(AV_PIX_FMT_YUV420P);
    m_videoSink->setVideoFrame(videoFrame);
}

void VideoItem::getAudioFrame(const QAVAudioFrame &frame)
{
    if (m_audioOutput == nullptr)
        return;
    m_audioOutput->play(frame);
}

void VideoItem::setAvPlayerOptions()
{
    // m_avPlayer->setLogsLevelBackend(32);    // 32 - debug

//     QMap<QString, QString> inputOptions = {
//                                            // Basic protocol options
//                                            {"protocol_whitelist", "tcp,rtp,rtsp,udp,https,http,tls,crypto,hls,prompeg,file"},
//                                            // {"rtsp_transport", "tcp"},              // Force TCP for stability
// //                                            {"stimeout", "8000000"},                // 5 sec timeout (microseconds)
// //                                            {"max_delay", "2000000"},               // Increased max delay
// //                                            {"reorder_queue_size", "0"},            // Disable reordering
// //                                            {"max_reconnect_delay", "10"},          // Max 10 sec between reconnects
// //                                            {"reconnect_on_network_error", "1"},
// //                                            {"reconnect_at_eof", "1"},
// //                                            {"reconnect_delay_max", "5"},

// //                                            // Buffer management
// //                                            {"buffer_size", "8388608"},             // 4MB buffer
// //                                            {"rtbufsize", "5000000"},               // 2MB realtime buffer
// //                                            {"fifo_size", "5000000"},               // Large FIFO for packet buffering
// //                                            {"max_analyze_duration", "10000000"},   // Longer analyze duration

// //                                            // Error resilience
// //                                            {"fflags", "+discardcorrupt+genpts+igndts+nobuffer+flush_packets"},
// //                                            {"err_detect", "explode+careful"},
// //                                            {"reconnect", "1"},                     // Enable auto-reconnect
// //                                            {"reconnect_streamed", "1"},
// //                                            // {"reconnect_delay_max", "30"},          // Max 30 sec reconnect delay

// //                                            // Packet loss handling
// //                                            {"drop_pkts_on_overflow", "1"},
// //                                            {"use_wallclock_as_timestamps", "1"},
// //                                            {"avoid_negative_ts", "make_zero"},

// //     // Hardware acceleration
// // #ifdef Q_OS_LINUX
// //                                            {"hwaccel", "cuda"},
// //                                            {"hwaccel_output_format", "cuda"},
// //                                            {"hwaccel_flags", "+allow_profile_mismatch"},
// // #elif defined(Q_OS_WIN)
// //                                            {"hwaccel", "d3d11va"},
// //                                            {"hwaccel_output_format", "d3d11va"},
// //                                            {"hwaccel_flags", "+allow_profile_mismatch"},
// // #endif

// //                                            // Decoder options
// //                                            {"skip_frame", "default"},              // Skip frames strategically
// //                                            {"skip_loop_filter", "all"},            // Skip loop filtering
// //                                            {"skip_idct", "all"},                   // Skip IDCT
// //                                            {"threads", "1"},                       // Single thread for stability
// //                                            {"max_error_rate", "1.0"},
// //                                            {"error_concealment", "1"},
//                                            };

// //     // inputOptions.insert("avioflags", "direct");
//     inputOptions.insert("flags", "+low_delay+autobsf+bitexact");
//     inputOptions.insert("analyzeduration", "20000000");  // 10 sec
//     inputOptions.insert("probesize", "20000000");        // 10 MB

//     // Additional options for corrupted video
//     inputOptions.insert("workaround_bugs", "1");
//     inputOptions.insert("strict", "very");
//     inputOptions.insert("discardcorrupt", "1");
//     inputOptions.insert("error_resilience", "1");
//     inputOptions.insert("resync_size", "100000000");

//     // For gray/black frame detection
//     inputOptions.insert("vf", "freezedetect=n=-60dB:d=2,"
//                               "blackframe=amount=99:threshold=32,"
//                               "idet");

//     // Protocol specific
//     inputOptions.insert("multiple_requests", "1");
//     inputOptions.insert("seekable", "0");
//     inputOptions.insert("reconnect_on_http_error", "404:403");

    QMap<QString, QString> inputOptions = {
        // QUALITY & STABILITY OPTIONS
        // {"rtsp_flags", "prefer_tcp"},
        // {"rtsp_transport", "tcp"},
        // {"stimeout", "10000000"},               // 10 sec timeout for stability
        // {"max_delay", "5000000"},               // Balanced delay for quality
        // {"reorder_queue_size", "50"},           // Enable reordering for packet recovery

        // MAXIMUM QUALITY DECODING
        {"fflags", "+genpts+igndts+discardcorrupt"},
        {"flags", "+autobsf+full_frame"},       // Full frame processing
        {"analyzeduration", "10000000"},        // 10 sec for proper stream analysis
        {"probesize", "50000000"},              // 50MB for complete codec detection

        // COMPREHENSIVE ERROR HANDLING
        // {"err_detect", "careful+compliant+aggressive"},
        // {"max_error_rate", "0.5"},              // Tolerant but not too permissive
        // {"error_concealment", "2"},             // Maximum error concealment
        // {"workaround_bugs", "1"},
        // {"strict", "normal"},                   // Balanced strictness

        // FPS & TIMING CONTROL
        {"vsync", "1"},                         // Frame rate conversion
        {"fps_mode", "auto"},                   // Automatic FPS handling
        {"framedrop", "0"},                     // NO frame dropping for quality
        {"avoid_negative_ts", "make_non_negative"},

        // COMPREHENSIVE GRAY/CORRUPTED FRAME HANDLING
        // {"vf", "format=yuv420p,"

        //        // "eq=contrast=1.1:brightness=0.05:saturation=1.4:gamma=1.05,"  // Color enhancement
        //        // "vibrance=intensity=1.2:rbal=1.1:gbal=1.0:bbal=1.1,"         // Color vibrance
        //        // "unsharp=luma_msize=5:luma_amount=0.8,"                      // Sharpness
        //        // "colorbalance=rs=0.1:gs=0.0:bs=0.1:rm=0.1:gm=0.0:bm=0.1:rh=0.05:gh=0.0:bh=0.05,"  // Color balance
        //        // "curves=preset=increase_contrast,"                           // Contrast curves

        //        "eq=contrast=1.2:brightness=0.02:saturation=1.8:gamma=1.1,"
        //        "vibrance=intensity=1.5:rbal=1.2:gbal=0.9:bbal=1.2,"
        //        "unsharp=luma_msize=5:luma_amount=1.2,"
        //        "colorbalance=rs=0.15:gs=0.0:bs=0.15:rm=0.15:gm=0.0:bm=0.15:rh=0.08:gh=0.0:bh=0.08,"
        //        "curves=preset=strong_contrast,"

        //        "graydetect=low=0.02:high=0.98:duration=1.0,"
        //        "freezedetect=n=-55dB:d=2,"
        //        "blackframe=amount=98:threshold=20,"
        //        "corrupted_frames=threshold=0.05,"
        //        "decimate=cycle=3:dupthresh=1.1,"
        //        "hqdn3d=4.0:3.0:6.0:4.5,"          // High quality denoise
        //        "sharpen=lr=0.5:ls=0.5:la=0.5"},    // Mild sharpening

        // BUFFER MANAGEMENT FOR STABILITY
        {"buffer_size", "16777216"},            // 16MB buffer
        {"rtbufsize", "10000000"},              // 10MB realtime buffer
        {"fifo_size", "10000000"},

        // RECONNECTION & NETWORK RESILIENCE
        // {"reconnect", "1"},
        // {"reconnect_at_eof", "1"},
        // {"reconnect_on_network_error", "1"},
        // {"reconnect_delay_max", "10"},
        // {"reconnect_streamed", "1"},
        // {"multiple_requests", "1"},

    // HARDWARE ACCELERATION (Quality Focused)
#ifdef Q_OS_LINUX
        {"hwaccel", "cuda"},
        {"hwaccel_output_format", "cuda"},
        {"hwaccel_flags", "+allow_profile_mismatch+high_quality"},
#elif defined(Q_OS_WIN)
        {"hwaccel", "d3d11va"},
        {"hwaccel_output_format", "d3d11va"},
        {"hwaccel_flags", "+allow_profile_mismatch+high_quality"},
#endif

        // THREADING FOR PERFORMANCE
        {"threads", "0"},                       // Auto-detect optimal threads
        {"thread_type", "frame+slice"},         // Multi-threading strategies

        // CODEC SPECIFIC OPTIMIZATIONS
        {"refs", "4"},                          // Reference frames
        {"g", "250"},                           // GOP size
        {"keyint_min", "25"},                   // Minimum keyframe interval
        {"qmin", "2"},                          // Minimum quality
        {"qmax", "69"}                          // Maximum quality
    };

    // Additional quality enhancements
    inputOptions.insert("color_primaries", "bt709");
    inputOptions.insert("color_trc", "bt709");
    inputOptions.insert("colorspace", "bt709");

    // Advanced error resilience
    inputOptions.insert("resync_size", "100000000");
    inputOptions.insert("drc_scale", "1.0");    // Dynamic range control

    // Protocol whitelist for maximum compatibility
    inputOptions.insert("protocol_whitelist",
                        "tcp,rtp,rtsp,udp,https,http,tls,crypto,hls,prompeg,file,rtmp,rtmps");
    // Set the optimized options
    m_avPlayer->setInputOptions(inputOptions);
}

void VideoItem::processAvPlaybackState(QAVPlayer::State playbackState)
{
    qDebug() << "state1 play curr: " << m_avPlayer->state() << " new: " << playbackState;
    switch(playbackState) {
    case QAVPlayer::PlayingState:
        qDebug() << "state1 playing: " << m_avPlayer->mediaStatus();

        break;

    case QAVPlayer::StoppedState:

        break;
    case QAVPlayer::PausedState:

        break;
    }
}

void VideoItem::processAvMediaStatus(QAVPlayer::MediaStatus mediaStatus)
{
    qDebug() << "state1 media curr: " << m_avPlayer->mediaStatus() << " new: " << mediaStatus;
    switch (mediaStatus) {
    case QAVPlayer::NoMedia:
        break;
    case QAVPlayer::LoadedMedia: {
        getAVStreamParams();
        // m_audioOutput = std::make_unique<QAVAudioOutput>();
        m_audioOutput->setVolume(1);
    } break;
    case QAVPlayer::EndOfMedia:

        break;
    case QAVPlayer::InvalidMedia:

        break;
    }
}

void VideoItem::processAvErrorStatus(QAVPlayer::Error error, const QString &errorString)
{
    qDebug() << errorString;
    qDebug() << "state1 media curr: FilterError: " << m_avPlayer->state() << m_avPlayer->mediaStatus();
    switch(error) {
    case QAVPlayer::FilterError:


        break;
    case QAVPlayer::NoError:
        qDebug() << "state1 media curr: NoError";
        break;
    case QAVPlayer::ResourceError:
        qDebug() << "state1 media curr: ResourceError";

        break;
    }
}

void VideoItem::getAVStreamParams()
{
    auto availableVideoStreams = m_avPlayer->availableVideoStreams();
    qDebug() << "Video streams:" << availableVideoStreams.size();
    qDebug() << "Audio streams:" << m_avPlayer->availableAudioStreams().size();
    for (auto &s : m_avPlayer->availableVideoStreams()) {
        const auto *codecPar = s.stream()->codecpar;
        qDebug() << "codec bitr: "
                 << (int)codecPar->bit_rate
                 << s.stream()->metadata
                 << (qreal(s.stream()->r_frame_rate.num)
                     / qreal(s.stream()->r_frame_rate.den == 0 ? 1 : s.stream()->r_frame_rate.den));
        qDebug() << "codec res: " << QSize(codecPar->width, codecPar->height);
        m_videoHeight = codecPar->height;
        m_videoWidth = codecPar->width;

        int codecBitrate = qreal(s.stream()->r_frame_rate.num) / qreal(s.stream()->r_frame_rate.den == 0 ? 1 : s.stream()->r_frame_rate.den);
        m_frameRate = qreal(s.stream()->avg_frame_rate.num) / qreal(s.stream()->avg_frame_rate.den == 0 ? 1 : s.stream()->avg_frame_rate.den);
        if ((codecBitrate < 35 && codecBitrate > 2) && (m_frameRate > 35 || m_frameRate < 3))
            m_frameRate = codecBitrate;
        if (m_frameRate < 1 || m_frameRate > 50)
            m_frameRate = 25;
        m_frameLowLimit = qCeil(m_frameRate * 0.65);
        m_frameHighLimit = qCeil(m_frameRate * 1.5);

        qDebug() << "codec framerate: " << (qreal(s.stream()->avg_frame_rate.num) / qreal(s.stream()->avg_frame_rate.den == 0 ? 1 : s.stream()->avg_frame_rate.den)) << m_frameRate << m_frameLowLimit << m_frameHighLimit;
        m_videoHeight = codecPar->height;
        m_videoWidth = codecPar->width;
        if (m_videoWidth != 0)
            break;
    }
}
