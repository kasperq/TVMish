#ifndef TVCONTROLLER_H
#define TVCONTROLLER_H

#include <QObject>
#include <QQmlApplicationEngine>


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

public slots:
    void openTVMode();
    void disconnectAllConnections();

private:
    QQmlApplicationEngine *m_engine;
    std::shared_ptr< Settings > m_sets;

    PlaylistController m_plContr {*m_engine};
    VideoPlayer m_video;
};

#endif // TVCONTROLLER_H
