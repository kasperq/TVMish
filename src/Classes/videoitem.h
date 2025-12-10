#ifndef VIDEOITEM_H
#define VIDEOITEM_H

#include <QQuickItem>
#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QAudioOutput>
#include <QWaitCondition>
#include <QMetaObject>

#include <QtAVPlayer/qavplayer.h>
#include <QtAVPlayer/qavvideoframe.h>
#include <QtAVPlayer/qavaudiooutput.h>
#include <QtAVPlayer/qaviodevice.h>
#include <QtAVPlayer/qtavplayerglobal.h>
#include <QtAVPlayer/qavstream.h>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QAbstractVideoSurface>
#include <private/qdeclarativevideooutput_p.h>
#else
#include <QVideoSink>
#include <QtMultimediaQuick/private/qquickvideooutput_p.h>
#endif

extern "C" {
#include <libavcodec/avcodec.h>
}

using VideoOutput = QQuickVideoOutput;

class VideoItem : public QQuickItem
{
    Q_OBJECT
public:
    VideoItem(QQuickItem *parent = nullptr);
    virtual ~VideoItem();

    QString source() const;
    void setSource(const QString &newSource);

signals:
//
    void sourceChanged();

protected:
    // void componentComplete() override;

private:
    std::unique_ptr<QAVPlayer> m_avPlayer;
    // std::shared_ptr<VideoOutput> m_videoOut;
    // std::shared_ptr<QVideoSink> m_videoSink;
    std::unique_ptr<QAVAudioOutput> m_audioOutput;

    VideoOutput* m_videoOut { nullptr };
    QVideoSink* m_videoSink { nullptr };

    QString m_source;
    int m_videoWidth { 0 };
    int m_videoHeight { 0 };
    int m_frameRate { 25 };
    int m_frameLowLimit { 16 };
    int m_frameHighLimit { 40 };

    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged FINAL)

private slots:
    void getVideoFrame(const QAVVideoFrame &frame);
    void getAudioFrame(const QAVAudioFrame &frame);
    void setAvPlayerOptions();
    void processAvPlaybackState(QAVPlayer::State playbackState);
    void processAvMediaStatus(QAVPlayer::MediaStatus mediaStatus);
    void processAvErrorStatus(QAVPlayer::Error error, const QString &errorString);
    void getAVStreamParams();
};

#endif // VIDEOITEM_H
