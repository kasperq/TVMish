#include "categories.h"

#include "./Classes/category.h"

Categories::Categories(QObject *parent)
    : QObject{parent}
{

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
    if (item.id() == oldItem.id()
        && item.name() == oldItem.name())
        return false;

    m_categories[index] = item;
    QString name = item.name();
    int id = item.id();

    return true;
}

void Categories::addItem(const int &idCategory, const QString &name)
{
    Category newCat;
    newCat.setId(idCategory);
    newCat.setName(name);

    m_categories.append(newCat);
}

int Categories::rowCount() const
{
    return m_categories.size();
}

int Categories::curCategoryId() const
{
    return m_curCategoryId;
}

void Categories::setCurCategoryId(const int &value)
{
    m_curCategoryId = value;
}

bool Categories::find(const int &id)
{
    return false;
}

bool Categories::find(const QString &name)
{

}

void Categories::add(const QString &name)
{

}

int Categories::getId(const QString &name)
{

}
