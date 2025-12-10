#ifndef TVCONTROLLER_H
#define TVCONTROLLER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QList>
#include <QUrl>

#include "playlistcontroller.h"
#include "Classes/videoplayer.h"

class TVController : public QObject
{
    Q_OBJECT
public:
    explicit TVController(QQmlApplicationEngine &engine);
    virtual ~TVController();

    void setSets(const Settings &value);
signals:
    void openFullScreenPlayer();
    void fullScreenClosed();
    void setFocus();
    void playFullScreenPlayer(QUrl playingUrl, QList<QUrl> tracks, int position);
    void playMiniPlayer(QUrl playingUrl, QList<QUrl> tracks, int position);
    void playChannel(QUrl url);
    void startPlayingChannel(QString url);

public slots:
    void openTVMode();
    void disconnectAllConnections();
    void showFullScreenPlayer(QUrl playingUrl, QList<QUrl> tracks, int position);
    void closeFullScreenPlayer(QUrl playingUrl, QList<QUrl> tracks, int position);
    void getFocus();

private:
    QQmlApplicationEngine *m_engine;
    std::shared_ptr< Settings > m_sets;

    PlaylistController m_plContr {*m_engine};
    VideoPlayer m_video;

private slots:
    void playChannelSlot(QUrl url);
};

#endif // TVCONTROLLER_H
