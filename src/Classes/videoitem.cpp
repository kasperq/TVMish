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

    QMap<QString, QString> inputOptions = {
                                           // Basic protocol options
                                           {"protocol_whitelist", "tcp,rtp,rtsp,udp,https,http,tls,crypto,hls,prompeg,file"},
                                           // {"rtsp_transport", "tcp"},              // Force TCP for stability
//                                            {"stimeout", "8000000"},                // 5 sec timeout (microseconds)
//                                            {"max_delay", "2000000"},               // Increased max delay
//                                            {"reorder_queue_size", "0"},            // Disable reordering
//                                            {"max_reconnect_delay", "10"},          // Max 10 sec between reconnects
//                                            {"reconnect_on_network_error", "1"},
//                                            {"reconnect_at_eof", "1"},
//                                            {"reconnect_delay_max", "5"},

//                                            // Buffer management
//                                            {"buffer_size", "8388608"},             // 4MB buffer
//                                            {"rtbufsize", "5000000"},               // 2MB realtime buffer
//                                            {"fifo_size", "5000000"},               // Large FIFO for packet buffering
//                                            {"max_analyze_duration", "10000000"},   // Longer analyze duration

//                                            // Error resilience
//                                            {"fflags", "+discardcorrupt+genpts+igndts+nobuffer+flush_packets"},
//                                            {"err_detect", "explode+careful"},
//                                            {"reconnect", "1"},                     // Enable auto-reconnect
//                                            {"reconnect_streamed", "1"},
//                                            // {"reconnect_delay_max", "30"},          // Max 30 sec reconnect delay

//                                            // Packet loss handling
//                                            {"drop_pkts_on_overflow", "1"},
//                                            {"use_wallclock_as_timestamps", "1"},
//                                            {"avoid_negative_ts", "make_zero"},

//     // Hardware acceleration
// #ifdef Q_OS_LINUX
//                                            {"hwaccel", "cuda"},
//                                            {"hwaccel_output_format", "cuda"},
//                                            {"hwaccel_flags", "+allow_profile_mismatch"},
// #elif defined(Q_OS_WIN)
//                                            {"hwaccel", "d3d11va"},
//                                            {"hwaccel_output_format", "d3d11va"},
//                                            {"hwaccel_flags", "+allow_profile_mismatch"},
// #endif

//                                            // Decoder options
//                                            {"skip_frame", "default"},              // Skip frames strategically
//                                            {"skip_loop_filter", "all"},            // Skip loop filtering
//                                            {"skip_idct", "all"},                   // Skip IDCT
//                                            {"threads", "1"},                       // Single thread for stability
//                                            {"max_error_rate", "1.0"},
//                                            {"error_concealment", "1"},
                                           };

//     // inputOptions.insert("avioflags", "direct");
    inputOptions.insert("flags", "+low_delay+autobsf+bitexact");
    inputOptions.insert("analyzeduration", "20000000");  // 10 sec
    inputOptions.insert("probesize", "20000000");        // 10 MB

    // Additional options for corrupted video
    inputOptions.insert("workaround_bugs", "1");
    inputOptions.insert("strict", "very");
    inputOptions.insert("discardcorrupt", "1");
    inputOptions.insert("error_resilience", "1");
    inputOptions.insert("resync_size", "100000000");

    // For gray/black frame detection
    inputOptions.insert("vf", "freezedetect=n=-60dB:d=2,"
                              "blackframe=amount=99:threshold=32,"
                              "idet");

    // Protocol specific
    inputOptions.insert("multiple_requests", "1");
    inputOptions.insert("seekable", "0");
    inputOptions.insert("reconnect_on_http_error", "404:403");

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
