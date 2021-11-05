#include "plfilegw.h"

PlFileGW::PlFileGW()
{

}

PlFileGW::~PlFileGW()
{

}

int *PlFileGW::rows()
{
    return &m_rows;
}

void PlFileGW::select(int *idPlaylist)
{
    q_select = new QSqlQuery();
    q_select->prepare("select playlist_file.ID_FILE, playlist_file.ID_PLAYLIST, playlist_file.FILE_NAME, "
                      "playlist_file.FILE_PATH, playlist_file.FILE_PATH_LOCAL, playlist_file.id_format, "
                      "playlist_file.is_available, format.naim format "
                      "from playlist_file "
                      "left join format on format.id_format = playlist_file.id_format "
                      "where playlist_file.ID_PLAYLIST = :id_playlist ");
    q_select->bindValue(":id_playlist", *idPlaylist);
    q_select->exec();
    calcRowCount();
}

void PlFileGW::insert(PlFile *newItem)
{
    q_insert = new QSqlQuery();
    q_insert->prepare("insert into PLAYLIST_FILE "
                      "(ID_FILE, ID_PLAYLIST, FILE_NAME, FILE_PATH, FILE_PATH_LOCAL, ID_FORMAT, IS_AVAILABLE) "
                      "values (:id_file, :id_playlist, :file_name, :file_path, :file_path_local,"
                      ":id_format, :is_available) ");
    q_insert->bindValue(":file_name", newItem->fileName());
    q_insert->bindValue(":file_path", newItem->filePath());
    q_insert->bindValue(":file_path_local", newItem->filePathLocal());
    q_insert->bindValue(":id_playlist", newItem->idPlaylist());
    q_insert->bindValue(":id_format", newItem->idFormat());
    q_insert->bindValue(":is_available", newItem->isAvailable());

    int newId = getMaxId() + 1;
    q_insert->bindValue(":id_file", newId);
    q_insert->exec();

    newItem->setIdFile(&newId);
}

void PlFileGW::modify(int *idFile, int *idPlaylist, QString *fileName, QString *filePath, QString *filePathLocal,
                      int *idFormat, bool *isAvailable)
{
    q_modify = new QSqlQuery();
    q_modify->prepare("update PLAYLIST_FILE "
                      "set "
                      "ID_FILE = :id_file, "
                      "ID_PLAYLIST = :id_playlist, "
                      "FILE_NAME = :file_name, "
                      "FILE_PATH = :file_path, "
                      "FILE_PATH_LOCAL = :file_path_local, "
                      "ID_FORMAT = :id_format, "
                      "IS_AVAILABLE = :is_available "
                      "where id_file = :id_file ");
    q_modify->bindValue(":id_file", *idFile);
    q_modify->bindValue(":id_playlist", *idPlaylist);
    q_modify->bindValue(":file_name", *fileName);
    q_modify->bindValue(":file_path", *filePath);
    q_modify->bindValue(":file_path_local", *filePathLocal);
    q_modify->bindValue(":id_format", *idFormat);
    q_modify->bindValue(":is_available", *isAvailable);
    q_modify->exec();
}

void PlFileGW::deleteRecord(int *idFile)
{
    q_delete = new QSqlQuery();
    q_delete->prepare("delete from PLAYLIST_FILE where id_file = :id_file ");
    q_delete->bindValue(":id_file", *idFile);
    q_delete->exec();
}

bool PlFileGW::findFormat(QString *format)
{
    q_temp = new QSqlQuery();
    q_temp->prepare("select format.id_format, format.naim "
                    "from format "
                    "where lower(trim(format.NAIM)) = lower(trim(:naim)) ");
    q_temp->bindValue(":naim", *format);
    q_temp->exec();
    q_temp->first();
    if (q_temp->isValid() && q_temp->value("id_format").toInt() != 0)
        return true;
    return false;
}

int PlFileGW::getFormatId(QString *format)
{
    if (findFormat(format))
        return q_temp->value("id_format").toInt();
    return 0;
}

QSqlQuery *PlFileGW::data() const
{
    return q_select;
}

void PlFileGW::calcRowCount()
{
    m_rows = 0;
    if (q_select->first()) {
        do {
            ++m_rows;
        }
        while (q_select->next());
    }
}

int PlFileGW::getMaxId()
{
    q_temp = new QSqlQuery();
    q_temp->exec("select max(playlist_file.id_file) max_id from playlist_file ");
    q_temp->first();
    return q_temp->value(0).toInt();
}
