#ifndef PLFILEGW_H
#define PLFILEGW_H

#include <QSqlQuery>

#include "./Classes/plfile.h"

class PlFileGW
{
public:
    PlFileGW();
    virtual ~PlFileGW();

    int *rows();

    void select(int *idPlaylist);
    void insert(PlFile *newItem);
    void modify(int *idFile, int *idPlaylist, QString *fileName, QString *filePath, QString *filePathLocal,
                int *idFormat, bool *isAvailable);
    void deleteRecord(int *idFile);
    bool findFormat(QString *format);
    int getFormatId(QString *format);

    QSqlQuery *data() const;
private:
    QSqlQuery *q_select;
    QSqlQuery *q_modify;
    QSqlQuery *q_insert;
    QSqlQuery *q_delete;
    QSqlQuery *q_temp;

    int m_rows;

    void calcRowCount();
    int getMaxId();
};

#endif // PLFILEGW_H
