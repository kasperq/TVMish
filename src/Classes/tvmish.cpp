#include <QMessageBox>
#include <QGuiApplication>

#include "tvmish.h"

TVmish::TVmish(QString *appPath) : m_appPath(appPath)
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
    db->disconnect();
//    delete *db;
//    delete *mainContr;
}

bool TVmish::startApp()
{
    db = new DataBase();
    if (db->connect()) {
        sets = new Settings();
        if (*m_appPath != sets->appPath())
            sets->setAppPath(m_appPath);

        mainContr = new MainController(db->db());
        mainContr->loadMainForm();        

        return true;
    }
    else {
        QMessageBox msgBox;
        msgBox.setText(tr("DB missing! Reinstall app."));
        msgBox.exec();

        return false;
    }
}
