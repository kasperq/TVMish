#include "categories.h"

#include "./Classes/category.h"

Categories::Categories(QObject *parent)
    : QObject{parent}
{
    initConnections();
}

Categories::~Categories()
{

}

QVector<Category> Categories::items() const
{
    return m_categories;
}

bool Categories::setItemAt(int index, Category &item)
{
    if (index < 0 || index >= m_categories.size())
        return false;

    const Category &oldItem = m_categories.at(index);
    if (item.idCategory() == oldItem.idCategory()
        && item.name() == oldItem.name()
        && item.isReadonly() == oldItem.isReadonly())
        return false;

    m_categories[index] = item;
    QString name = item.name();
    int id = item.idCategory();
    bool isReadonly = item.isReadonly();

    emit itemEdited(index, id, name, isReadonly);

    return true;
}

void Categories::addItem(const int &idCategory, const QString &name, const bool &isReadonly)
{
    Category newCat;
    newCat.setIdCategory(idCategory);
    newCat.setName(name);
    newCat.setIsReadonly(isReadonly);

    m_categories.append(newCat);
}

void Categories::clear()
{
    if (m_categories.size() > 0)
        m_categories.clear();
    emit rowCountChanged(rowCount());
}

int Categories::rowCount() const
{
    return m_categories.size();
}

int Categories::curCategoryId() const
{
    return m_curIdCategory;
}

void Categories::setCurCategoryId(const int &value)
{
    m_curIdCategory = value;
}

//bool Categories::find(const int &id)
//{
//    return false;
//}

//bool Categories::find(const QString &name)
//{

//}

//void Categories::add(const QString &name)
//{

//}

//int Categories::getIdCategory(const QString &name)
//{

//}

void Categories::open()
{
    m_curIdCategory = 0;
    if (m_categories.size() > 0)
        emit selectItem(0);
    emit rowCountChanged(rowCount());
}

void Categories::scroll(int index)
{
    m_curIdCategory = m_categories.at(index).idCategory();
    emit categoriesScrolled(m_curIdCategory);
}

void Categories::initConnections()
{

}
