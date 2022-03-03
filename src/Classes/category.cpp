#include "category.h"

Category::Category()
{

}

Category::~Category()
{

}

int Category::id() const
{
    return m_id;
}

void Category::setId(const int &value)
{
    m_id = value;
}

QString Category::name() const
{
    return m_name;
}

void Category::setName(const QString &value)
{
    m_name = value;
}
