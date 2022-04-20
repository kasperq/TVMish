#include "category.h"

Category::Category()
{

}

Category::~Category()
{

}

int Category::idCategory() const
{
    return m_idCategory;
}

void Category::setIdCategory(const int &value)
{
    m_idCategory = value;
}

QString Category::name() const
{
    return m_name;
}

void Category::setName(const QString &value)
{
    m_name = value;
}

bool Category::isReadonly() const
{
    return m_isReadonly;
}

void Category::setIsReadonly(const bool &newIsReadonly)
{
    m_isReadonly = newIsReadonly;
}
