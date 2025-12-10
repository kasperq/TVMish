#ifndef EPGCONTROLLER_H
#define EPGCONTROLLER_H

#include <QObject>
#include <QQmlApplicationEngine>

#include "./Classes/settings.h"
#include "./Classes/epgs.h"
#include "./Gateways/epggw.h"

class EpgController : public QObject
{
    Q_OBJECT
public:
    explicit EpgController(QQmlApplicationEngine &engine);
    virtual ~EpgController();

    void disconnectAllConnections();
    void openEpgManager();

    void setSets(const Settings &value);

signals:
    void epgsAdded();

public slots:
    void addItemsFromDbToEpgs();

private:
    QQmlApplicationEngine *m_engine;

    std::shared_ptr< Settings > m_sets;

    EpgGW m_epgGW {this};
    Epgs m_epgs;

    void initEpgsConnections();
    void openEpgs();
};

#endif // EPGCONTROLLER_H
