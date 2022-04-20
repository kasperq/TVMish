#include "categorygw.h"

#include <QFuture>
#include <exception>

#include "./DB/dbst.h"


CategoryGW::CategoryGW(QObject *parent)
    : QObject{parent}
{

}

CategoryGW::~CategoryGW()
{

}

QSqlQuery *CategoryGW::data() const
{
    return const_cast<QSqlQuery*>(&q_select);
}

int CategoryGW::rows() const
{
    return m_rows;
}

void CategoryGW::select()
{    
    DBst::getInstance().startTransDBdef();
    q_select = QSqlQuery(DBst::getInstance().db_def());
    q_select.prepare("select category.id_category, category.name, category.is_readonly "
                     "from category ");
    q_select.exec();
    calcRowCount();
    emit selected();
}

void CategoryGW::insert(const int &index, const QString &name, const bool &waitForResult)
{    
    DBst::getInstance().startTransDBdef();
    q_insert = QSqlQuery(DBst::getInstance().db_def());
    q_insert.prepare("insert into CATEGORY "
                     "(ID_CATEGORY, NAME) "
                     "values (:id_category, :name) ");
    q_insert.bindValue(":name", name);

    int newId = getMaxId() + 1;
    q_insert.bindValue(":id_category", newId);

//    if (waitForResult) {
//        q_insert.exec();
//        q_insert.finish();
//        emit inserted(index, newId);
//    } else {
//        DBst::getInstance().execAndCheck(q_insert).then([this, newId, index](bool result) {
//            if (result) {
    if (q_insert.exec()) {
                DBst::getInstance().commitDbDef();
                q_insert.finish();
                emit inserted(index, newId);
            }
//        });
//    }
}

void CategoryGW::modify(const int &index, const int &idCategory, const QString &name)
{    
    DBst::getInstance().startTransDBdef();
    q_modify = QSqlQuery(DBst::getInstance().db_def());
    q_modify.prepare("update CATEGORY "
                      "set "
                      "ID_CATEGORY = :id_category, "
                      "NAME = :name "
                      "where id_category = :id_category ");
    q_modify.bindValue(":id_category", idCategory);
    q_modify.bindValue(":name", name);

//    DBst::getInstance().execAndCheck(q_modify).then([this, index](bool result) {
//        if (result) {
    if (q_modify.exec()) {
            DBst::getInstance().commitDbDef();
            q_modify.finish();
            emit modified(index);
        }
//    });
}

void CategoryGW::deleteRecord(const int &index, const int &idCategory)
{  
    DBst::getInstance().startTransDBdef();
    q_delete = QSqlQuery(DBst::getInstance().db_def());
    q_delete.prepare("delete from CATEGORY where id_category = :id_category ");
    q_delete.bindValue(":id_category", idCategory);

//    DBst::getInstance().execAndCheck(q_delete).then([this, idCategory, index](bool result) {
//        if (result) {
    if (q_delete.exec()) {
            DBst::getInstance().commitDbDef();
            q_delete.finish();
            emit deleted(index, idCategory);
        }
//    });
}

bool CategoryGW::find(const int &index, const QString &name)
{    
    DBst::getInstance().startTransDBdef();
    q_temp = QSqlQuery(DBst::getInstance().db_def());    
    q_temp.prepare("select category.id_category, category.name "
                   "from category "
                   "where lower(trim(category.name)) = cast(lower(trim(:name)) as varchar(100)) ");
    q_temp.bindValue(":name", name);
    q_temp.exec();

    q_temp.first();
    int id {};
    bool isFound {false};
    if (q_temp.isValid()) {
        id = q_temp.value("id_category").toInt();    
        isFound = true;
        emit found(index, isFound, id);
        q_temp.finish();
        return true;
    }

    q_temp.finish();
    emit found(index, isFound, id);
    return false;
}

int CategoryGW::findIdCategory(const QString &name)
{    
    DBst::getInstance().startTransDBdef();
    q_temp = QSqlQuery(DBst::getInstance().db_def());
    q_temp.prepare("select category.id_category, category.name "
                   "from category "
                   "where lower(trim(category.name)) = cast(lower(trim(:name)) as varchar(100)) ");
    q_temp.bindValue(":name", name);
    q_temp.exec();
    q_temp.first();
    int id {};
    if (q_temp.isValid()) {
        id = q_temp.value("id_category").toInt();
    }
    q_temp.finish();
    return id;
}

void CategoryGW::calcRowCount()
{
    m_rows = 0;    
    if (q_select.first()) {
        do {
            ++m_rows;
        }
        while (q_select.next());
        q_select.first();
    }
}

int CategoryGW::getMaxId()
{    
    DBst::getInstance().startTransDBdef();
    q_temp = QSqlQuery(DBst::getInstance().db_def());
    q_temp.prepare("select max(category.id_category) max_id from category ");
    int maxId {};
    if (q_temp.exec()) {
        q_temp.first();
        maxId = q_temp.value(0).toInt();
        q_temp.finish();
    }
    return maxId;
}
