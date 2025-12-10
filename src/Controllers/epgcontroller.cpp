#include "epgcontroller.h"

#include <QDateTime>
#include <QQmlContext>

EpgController::EpgController(QQmlApplicationEngine &engine)
    : m_engine{&engine}
{

}

EpgController::~EpgController()
{
    m_engine = nullptr;
}

void EpgController::disconnectAllConnections()
{
    m_epgGW.disconnect();
    m_epgs.disconnect();
}

void EpgController::openEpgManager()
{
    m_sets->getSets();
    initEpgsConnections();
    openEpgs();
}

void EpgController::setSets(const Settings &value)
{
    m_sets = std::make_shared< Settings > (value);
}

void EpgController::addItemsFromDbToEpgs()
{
    int idEpg;
    QString naim;
    QUrl url;
    QDateTime dateTimeUpdate;
    QDateTime dateTimeBegin;
    QDateTime dateTimeEnd;

    m_epgs.clear();

    if (m_epgGW.rows() > 0) {
        m_epgGW.data()->first();
        do {
            idEpg = m_epgGW.data()->value("id_epg").toInt();
            naim = m_epgGW.data()->value("naim").toString();
            url = m_epgGW.data()->value("url").toUrl();
            dateTimeUpdate = m_epgGW.data()->value("date_time_update").toDateTime();
            dateTimeBegin = m_epgGW.data()->value("date_time_begin").toDateTime();
            dateTimeEnd = m_epgGW.data()->value("date_time_end").toDateTime();

            m_epgs.addItem(idEpg, naim, url, dateTimeUpdate, dateTimeBegin, dateTimeEnd);
        } while(m_epgGW.data()->next());
    }
    emit epgsAdded();
}

void EpgController::initEpgsConnections()
{
    connect(&m_epgGW, &EpgGW::selected, this, &EpgController::addItemsFromDbToEpgs);
    connect(this, &EpgController::epgsAdded, &m_epgs, &Epgs::open);

    connect(&m_epgs, &Epgs::itemAppended, &m_epgGW, &EpgGW::insert);
    connect(&m_epgGW, &EpgGW::inserted, &m_epgs, &Epgs::setNewIdEpg);
    connect(&m_epgs, &Epgs::itemEdited, &m_epgGW, &EpgGW::modify);
    connect(&m_epgs, &Epgs::epgDeleted, &m_epgGW, &EpgGW::deleteRecord);

}

void EpgController::openEpgs()
{
//    m_epgs.setSets(*m_sets);
    m_epgs.initConnections();
    m_engine->rootContext()->setContextProperty(QStringLiteral("epgs"), &m_epgs);
    m_epgGW.select();
}
