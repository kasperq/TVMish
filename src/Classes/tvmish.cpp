#include <QMessageBox>
#include <QGuiApplication>

#include "tvmish.h"

TVmish::TVmish(QString &appPath) : m_appPath(appPath)
{
//    m_url = "localhost";
//#ifdef QT_DEBUG
//    m_dbName = "d:/Projects/TVmish/db/TVDB.db3";
//#else
//    m_dbName = QGuiApplication::applicationDirPath().toStdString() + "/TVDB.db3";
//#endif
}

TVmish::~TVmish()
{
    db.disconnect();
//    delete *db;
//    delete *mainContr;
}

bool TVmish::startApp()
{
//    db = new DataBase();
    if (db.connect()) {
        m_sets.setDB(db.db());
//        m_sets = new Settings(db->db());
        if (m_appPath != m_sets.appPath())
            m_sets.setAppPath(m_appPath);

//        m_mainContr = new MainController(db.db());
//        m_mainContr.setDb(db);
        m_mainContr.loadMainForm();
        qDebug() << "TVMIsh: " << db.db();
        return true;
    }
    else {
        QMessageBox msgBox;
        msgBox.setText(tr("DB missing! Reinstall app."));
        msgBox.exec();

        return false;
    }
}
