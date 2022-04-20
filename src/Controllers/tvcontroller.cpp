#include "tvcontroller.h"

#include <QQmlContext>



TVController::TVController(QQmlApplicationEngine &engine) : m_engine(&engine)
{

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
    m_engine->rootContext()->setContextProperty(QStringLiteral("_videoPlayer"), &m_video);
    m_plContr.setSets(*m_sets);
    m_video.setSets(*m_sets);
    m_plContr.openPlaylistViewer();
}

void TVController::disconnectAllConnections()
{
    m_plContr.disconnectAllConnections();

}
