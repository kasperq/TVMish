#ifndef PLFILEGW_H
#define PLFILEGW_H

#include <QObject>
#include <QSqlQuery>

#include "./Classes/plfile.h"
#include "./Classes/additional.h"

class PlFileGW : public QObject
{
    Q_OBJECT
public:
    PlFileGW(QObject *parent = nullptr/*, QSqlDatabase db = QSqlDatabase()*/);
    virtual ~PlFileGW();

    int rows() const;


    void insert(PlFile *newItem);
    bool findFormat(const QString &format);

    QSqlQuery *data() const;


signals:
    void selected();
    void inserted(const int &index, const int &newId);
    void modified(const int &index);
    void deleted(const int &index, const int &id);
    void deletedAll(const int &index, const int &idPlaylist);
    void formatId(const int &index, const int &id);
    void extensionChecked(const int &index, QString &fullFilePath, QString &newFilePath, const QString &extension,
                          const bool &isValid, const int &idFormat, const uint &sender);

public slots:
    void select(const int &idPlaylist);
    void insert(const int &index, const int &idPlaylist, const QString &fileName, const QString &filePath,
                const QString &filePathLocal, const int &idFormat, const bool &isAvailable);
    void modify(const int &index, const int &idFile, const int &idPlaylist, const QString &fileName, const QString &filePath,
                const QString &filePathLocal, const int &idFormat, const bool &isAvailable);
    void deleteRecord(const int &index, const int &idFile);
    void deleteAll(const int &index, const int &idPlaylist);
    int getFormatId(const int &index, const QString &format);

    void checkFileExtension(const int &index, QString &fullFilePath, QString &newFilePath, const QString &extension, const uint &sender);

private:
//    QSqlDatabase m_db;

    QSqlQuery q_select;
    QSqlQuery q_modify;
    QSqlQuery q_insert;
    QSqlQuery q_delete;
    QSqlQuery q_temp;

    int m_rows;

    void calcRowCount();
    int getMaxId();
};

#endif // PLFILEGW_H
