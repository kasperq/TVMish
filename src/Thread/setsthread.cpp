#include "setsthread.h"

//#include "Classes/settings.h"

//SetsThread::SetsThread(DataBase &db) : m_db(std::make_shared< DataBase > (db))
//{

//}

SetsThread::SetsThread()
{

}

SetsThread::SetsThread(QSqlDatabase &qdb) : m_qdb(std::make_shared< QSqlDatabase > (qdb))
{

}

//void SetsThread::setDb(QSqlDatabase &qdb)
//{
//    m_qdb = std::make_shared< QSqlDatabase > (qdb);
//}

void SetsThread::modify(QString &appPath, QString &curPlDir)
{
//    Settings m_sets {m_db->db()};
//    Settings m_sets(m_qdb);
//    m_sets.modifyThr(appPath, curPlDir);
    qDebug() << "SetsThread modified";

    if (m_qdb) {
        qDebug() << "sets modify";
        q_temp = QSqlQuery(*m_qdb);
        q_temp.prepare("update settings "
                    "set app_dir = :app_dir, "
                    "current_pl_dir = :current_pl_dir "
                    "where id_sets = :id_sets ");
        q_temp.bindValue(":app_dir", appPath);
        q_temp.bindValue(":current_pl_dir", curPlDir);
        q_temp.exec();
        qDebug() << "sets modify ended";
        q_temp.finish();
    }
}

void SetsThread::run()
{
    qDebug() << "SetsThread is running";
}
