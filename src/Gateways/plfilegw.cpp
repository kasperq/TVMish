#include "plfilegw.h"

#include "DB/dbst.h"

#include <QSqlError>

PlFileGW::PlFileGW(QObject *parent/*, QSqlDatabase db*/) : QObject(parent)/*, m_db(db)*/
{

}

PlFileGW::~PlFileGW()
{

}

int PlFileGW::rows() const
{
    return m_rows;
}

void PlFileGW::select(const int &idPlaylist)
{
    DBst::getInstance().startTransDBdef();
    q_select = QSqlQuery(/*m_db*/ DBst::getInstance().db_def());
    q_select.prepare("select playlist_file.ID_FILE, playlist_file.ID_PLAYLIST, playlist_file.FILE_NAME, "
                     "playlist_file.FILE_PATH, playlist_file.FILE_PATH_LOCAL, playlist_file.id_format, "
                     "playlist_file.is_available, format.naim format "
                     "from playlist_file "
                     "left join format on format.id_format = playlist_file.id_format "
                     "where playlist_file.ID_PLAYLIST = :id_playlist ");
    q_select.bindValue(":id_playlist", idPlaylist);

//    QFuture<QSqlQuery> future = DBst::getInstance().execAndGetQuery(q_select);
//    qDebug() << "";
//    q_select = future.result();
    m_rows = 0;
    if (q_select.exec())
        calcRowCount();
//    qDebug() << "GW: selected: " << m_rows;
    emit selected();
}

void PlFileGW::insert(PlFile *newItem)
{
    DBst::getInstance().startTransDBdef();
    q_insert = QSqlQuery(/*m_db*/ DBst::getInstance().db_def());
    q_insert.prepare("insert into PLAYLIST_FILE "
                     "(ID_FILE, ID_PLAYLIST, FILE_NAME, FILE_PATH, FILE_PATH_LOCAL, ID_FORMAT, IS_AVAILABLE) "
                     "values (:id_file, :id_playlist, :file_name, :file_path, :file_path_local,"
                     ":id_format, :is_available) ");
    q_insert.bindValue(":file_name", newItem->fileName());
    q_insert.bindValue(":file_path", newItem->filePath());
    q_insert.bindValue(":file_path_local", newItem->filePathLocal());
    q_insert.bindValue(":id_playlist", newItem->idPlaylist());
    q_insert.bindValue(":id_format", newItem->idFormat());
    q_insert.bindValue(":is_available", newItem->isAvailable());

    int newId = getMaxId() + 1;
    q_insert.bindValue(":id_file", newId);

    DBst::getInstance().execAndGetQuery(q_insert);
    if (q_insert.exec()) {
        DBst::getInstance().commitDbDef();
//    q_insert.exec();

        newItem->setIdFile(newId);
    }
}

void PlFileGW::insert(const int &index, const int &idPlaylist, const QString &fileName, const QString &filePath,
                      const QString &filePathLocal, const int &idFormat, const bool &isAvailable)
{
    DBst::getInstance().startTransDBdef();
    q_insert = QSqlQuery(/*m_db*/ DBst::getInstance().db_def());
    q_insert.prepare("insert into PLAYLIST_FILE "
                     "(ID_FILE, ID_PLAYLIST, FILE_NAME, FILE_PATH, FILE_PATH_LOCAL, ID_FORMAT, IS_AVAILABLE) "
                     "values (:id_file, :id_playlist, :file_name, :file_path, :file_path_local,"
                     ":id_format, :is_available) ");
    q_insert.bindValue(":file_name", fileName);
    q_insert.bindValue(":file_path", filePath);
    q_insert.bindValue(":file_path_local", filePathLocal);
    q_insert.bindValue(":id_playlist", idPlaylist);
    q_insert.bindValue(":id_format", idFormat);
    q_insert.bindValue(":is_available", isAvailable);

    int newId = getMaxId() + 1;
    q_insert.bindValue(":id_file", newId);

//    DBst::getInstance().execAndCheck(q_insert).then([this, newId, index](bool result) {
//        if (result) {
    if (q_insert.exec()) {
            DBst::getInstance().commitDbDef();
            q_insert.finish();            
            emit inserted(index, newId);            
        }
//    });
//    q_insert.exec();
//    q_insert.finish();
//    emit inserted(index, newId);
}

void PlFileGW::modify(const int &index, const int &idFile, const int &idPlaylist, const QString &fileName,
                      const QString &filePath, const QString &filePathLocal, const int &idFormat, const bool &isAvailable)
{    
    DBst::getInstance().startTransDBdef();
    q_modify = QSqlQuery(/*m_db*/ DBst::getInstance().db_def());
    q_modify.prepare("update PLAYLIST_FILE "
                      "set "
                      "ID_FILE = :id_file, "
                      "ID_PLAYLIST = :id_playlist, "
                      "FILE_NAME = :file_name, "
                      "FILE_PATH = :file_path, "
                      "FILE_PATH_LOCAL = :file_path_local, "
                      "ID_FORMAT = :id_format, "
                      "IS_AVAILABLE = :is_available "
                      "where id_file = :id_file ");
    q_modify.bindValue(":id_file", idFile);
    q_modify.bindValue(":id_playlist", idPlaylist);
    q_modify.bindValue(":file_name", fileName);
    q_modify.bindValue(":file_path", filePath);
    q_modify.bindValue(":file_path_local", filePathLocal);
    q_modify.bindValue(":id_format", idFormat);
    q_modify.bindValue(":is_available", isAvailable);

//    DBst::getInstance().execAndCheck(q_modify).then([this, index](bool result) {
//        if (result) {
    if (q_modify.exec()) {
            DBst::getInstance().commitDbDef();
            q_modify.finish();
            emit modified(index);            
        }
//    });
//    q_modify.exec();
//    q_modify.finish();
//    emit modified(index);
}

void PlFileGW::deleteRecord(const int &index, const int &idFile)
{
    DBst::getInstance().startTransDBdef();
    q_delete = QSqlQuery(/*m_db*/ DBst::getInstance().db_def());
    q_delete.clear();
    q_delete.prepare("delete from PLAYLIST_FILE where id_file = :id_file ");
    q_delete.bindValue(":id_file", idFile);    

//    DBst::getInstance().execAndCheck(q_delete).then([this, idFile, index](bool result) {
//        if (result) {
    if (q_delete.exec()) {
            qDebug() << "PlFileGW: delete res: " << DBst::getInstance().commitDbDef();;            
            q_delete.finish();
            emit deleted(index, idFile);
        }
//    });

//    q_delete.exec();
//    q_delete.finish();
    //    emit deleted(index, idFile);
}

void PlFileGW::deleteAll(const int &index, const int &idPlaylist)
{
    DBst::getInstance().startTransDBdef();
    q_delete = QSqlQuery(DBst::getInstance().db_def());
    q_delete.clear();
    q_delete.prepare("delete from PLAYLIST_FILE where id_playlist = :id_playlist ");
    q_delete.bindValue(":id_file", idPlaylist);

//    DBst::getInstance().execAndCheck(q_delete).then([this, idPlaylist, index](bool result) {
//        if (result) {
    if (q_delete.exec()) {
            DBst::getInstance().commitDbDef();
            q_delete.finish();
            emit deletedAll(index, idPlaylist);
        }
//    });
}

bool PlFileGW::findFormat(const QString &format)
{        
    qDebug() << "plgw: findFormat: " << format;
    DBst::getInstance().startTransDBdef();
    q_temp = QSqlQuery(/*m_db*/ DBst::getInstance().db_def());
    q_temp.clear();
    q_temp.prepare("select format.id_format, format.naim "
                    "from format "
                    "where lower(trim(format.NAIM)) = lower(trim(:naim)) ");
    q_temp.bindValue(":naim", format);

//    QFuture<QSqlQuery> future = DBst::getInstance().execAndGetQuery(q_temp);
//    qDebug() << "";
//    qDebug() << "";
//    q_temp = future.result();
//    qDebug() << "";
    if (q_temp.exec()) {
        q_temp.first();
        qDebug() << "plGw: findedFormat: " << q_temp.value("id_format").toInt();
        if (q_temp.isValid() && q_temp.value("id_format").toInt() != 0)
            return true;
        else
            return false;
    } else {
        qDebug() << "findFormat error: " << q_temp.lastError();
    }
    q_temp.finish();
    return false;
}

int PlFileGW::getFormatId(const int &index, const QString &format)
{
    int id {};
    qDebug() << "plGw: index: " << index << " format: " << format;
    if (findFormat(format))
        id = q_temp.value("id_format").toInt();
    emit formatId(index, id);
    return id;
}

void PlFileGW::checkFileExtension(const int &index, QString &fullFilePath, QString &newFilePath, const QString &extension, const uint &sender)
{
    int idFormat {getFormatId(index, extension)};
    int isValid {false};
    if (idFormat > 0)
        isValid = true;
    emit extensionChecked(index, fullFilePath, newFilePath, extension, isValid, idFormat, sender);
}

QSqlQuery *PlFileGW::data() const
{
    return const_cast<QSqlQuery*>(&q_select);
}

void PlFileGW::calcRowCount()
{
    m_rows = 0;
    if (q_select.first()) {
        do {
            ++m_rows;
        }
        while (q_select.next());
    }
}

int PlFileGW::getMaxId()
{
    DBst::getInstance().startTransDB();
    q_temp = QSqlQuery(/*m_db*/ DBst::getInstance().db_def());
    q_temp.clear();
    q_temp.prepare("select max(playlist_file.id_file) max_id from playlist_file ");

//    QFuture<QSqlQuery> future = DBst::getInstance().execAndGetQuery(q_temp);
//    qDebug() << "";
//    q_temp = future.result();
    if (q_temp.exec()) {
        q_temp.first();
        int maxId {};
        if (q_temp.isValid())
            maxId = q_temp.value(0).toInt();

        qDebug() << "PlFileGW::getMaxId(): " << maxId;
        return maxId;
    } else {
        qDebug() << "getMaxId error: " << q_temp.lastError();
    }
    q_temp.finish();
    return 0;
}
