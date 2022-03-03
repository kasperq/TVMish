#ifndef CATEGORYGW_H
#define CATEGORYGW_H

#include <QObject>
#include <QSqlQuery>

class CategoryGW : public QObject
{
    Q_OBJECT
public:
    explicit CategoryGW(QObject *parent = nullptr);
    virtual ~CategoryGW();

    QSqlQuery *data() const;
    int rows() const;

signals:
    void selected();
    void inserted(const int &index, const int &newId);
    void modified(const int &index);
    void deleted(const int &index, const int &id);
    void categoryId(const int &index, const int &id);
    void found(const int &index, const bool &isFinded, const int &id);

public slots:
    void select();
    void insert(const int &index, const QString &name, const bool &waitForResult);
    void modify(const int &index, const int &idCategory, const QString &name);
    void deleteRecord(const int &index, const int &idCategory);

    bool find(const int &index, const QString &name);
    int findIdCategory(const QString &name);

private:
    QSqlQuery q_select;
    QSqlQuery q_modify;
    QSqlQuery q_insert;
    QSqlQuery q_delete;
    QSqlQuery q_temp;

    int m_rows {};

    void calcRowCount();
    int getMaxId();
};

#endif // CATEGORYGW_H
