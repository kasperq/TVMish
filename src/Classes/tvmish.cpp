#include <QMessageBox>
#include <QGuiApplication>

#include "tvmish.h"

TVmish::TVmish(QString &appPath, QObject *parent) : QObject(parent), m_appPath(appPath)
{
//    m_url = "localhost";
//#ifdef QT_DEBUG
//    m_dbName = "d:/Projects/TVmish/db/TVDB.db3";
//#else
//    m_dbName = QGuiApplication::applicationDirPath().toStdString() + "/TVDB.db3";
//#endif
    qDebug() << "tvmish()";
    connect(this, SIGNAL(readyForLoadApp()), this, SLOT(loadApp()));
}

TVmish::~TVmish()
{
    DBst::getInstance().close();
    DBst::getInstance().closeInstance();
//    db.disconnect();

//    delete *db;
//    delete *mainContr;
}

bool TVmish::startApp()
{
    DBst::getInstance().isPrepared().then([this] (bool res) {
        qDebug() << "tvmish is prepared dbst " << res;
        if (res) {
            DBst::getInstance().open().then([this](bool openResult) {
                qDebug() << "tvmish: dbst is " << openResult;

                emit readyForLoadApp();
            });
        } else {
            QMessageBox msgBox;
            msgBox.setText(tr("DB missing! Reinstall app."));
            msgBox.exec();
        }
    });
    return true;
//    m_dbst.isPrepared().then([this](bool res) {
//        qDebug() << "tvmish is prepared dbst " << res;
//        if (res) {
//            m_dbst.open().then([this](bool openResult) {
//                qDebug() << "tvmish: dbst is " << openResult;
//                if (openResult) {
//                    QSqlQuery query(m_dbst.db());
//                    query.prepare("select settings.app_dir, settings.current_pl_dir, settings.id_sets from settings ");
//                    m_dbst.select(query).then([](QSqlQuery q) {
//                        qDebug() << "selected ";
//                        q.first();
//                        qDebug() << "query: " << q.value("app_dir").toString();
//                    });

//                    QSqlQuery query(m_dbst.db());
//                    query.prepare("insert into SETTINGS "
//                                "(id_sets, app_dir, current_pl_dir) "
//                                "values (5, '123', '456' ) ");

//                    m_dbst.insert(query);
//                } else {

//                }
//            });
//        }
//    });

//    db = new DataBase();
//    if (db.connect()) {
//        m_sets.setDB(db.db());
//        m_sets = new Settings(db->db());
//        if (m_appPath != m_sets.appPath())
//            m_sets.setAppPath(m_appPath);

//        m_mainContr = new MainController(db.db());
//        m_mainContr.setDb(db);
//        m_mainContr.loadMainForm();
//        qDebug() << "TVMIsh: " << db.db();
//        return true;
//    }
//    else {
//        QMessageBox msgBox;
//        msgBox.setText(tr("DB missing! Reinstall app."));
//        msgBox.exec();

//        return false;
    //    }
}

void TVmish::loadApp()
{
    m_sets.getSets();
    if (m_appPath != m_sets.appPath())
        m_sets.setAppPath(m_appPath);
    m_mainContr.loadMainForm();
}
