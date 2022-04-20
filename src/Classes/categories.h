#ifndef CATEGORIES_H
#define CATEGORIES_H

#include <QObject>
#include <QString>

#include "./Classes/category.h"

class Categories : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int rowCount READ rowCount NOTIFY rowCountChanged)
public:
    explicit Categories(QObject *parent = nullptr);
    virtual ~Categories();

    QVector< Category > items() const;
    bool setItemAt(int index, Category &item);
    void addItem(const int &idCategory, const QString &name, const bool &isReadonly);
    void clear();
    int rowCount() const;

    int curCategoryId() const;
    void setCurCategoryId(const int &value);

//    bool find(const int &id);
//    bool find(const QString &name);
//    void add(const QString &name);
//    int getIdCategory(const QString &name);

signals:
    void beforeItemAppended();
    void afterItemAppended();

    void itemEdited(const int &index, const int &id, const QString &name, const bool &isReadonly);
    void categoriesScrolled(const int &idCategory);

    // for external use in QML
    void rowCountChanged(int rows);
    void selectItem(int index);
    void listChanged();

public slots:
    void open();
    void scroll(int index);    

private:
    QVector< Category > m_categories;

    int m_curIdCategory {};

    void initConnections();
};

#endif // CATEGORIES_H
