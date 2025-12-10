#include "tvcontroller.h"

#include <QQmlContext>

#include "Classes/videoitem.h"



TVController::TVController(QQmlApplicationEngine &engine) : m_engine(&engine)
{
    connect(this, &TVController::playChannel, this, &TVController::playChannelSlot);
}

TVController::~TVController()
{

}

void TVController::setSets(const Settings &value)
{
    m_sets = std::make_shared< Settings > (value);
}

void TVController::openTVMode()
{
    qmlRegisterType<VideoItem>("ACME.VideoItem", 1, 0, "VideoItem");
    m_engine->rootContext()->setContextProperty(QStringLiteral("_videoPlayer"), &m_video);
    m_engine->rootContext()->setContextProperty(QStringLiteral("_tvContr"), this);
    m_plContr.setSets(*m_sets);
    m_video.setSets(*m_sets);
    m_plContr.openPlaylistViewer();
}

void TVController::disconnectAllConnections()
{
    m_plContr.disconnectAllConnections();

}

void TVController::showFullScreenPlayer(QUrl playingUrl, QList<QUrl> tracks, int position)
{
    emit openFullScreenPlayer();
    emit playFullScreenPlayer(playingUrl, tracks, position);
}

void TVController::closeFullScreenPlayer(QUrl playingUrl, QList<QUrl> tracks, int position)
{
    qDebug() << "TVController::closeFullScreenPlayer";
    emit fullScreenClosed();
    emit playMiniPlayer(playingUrl, tracks, position);
}

void TVController::getFocus()
{
    emit setFocus();
}

void TVController::playChannelSlot(QUrl url)
{
    qDebug() << "start " << url;
    emit startPlayingChannel(url.toString());
}
