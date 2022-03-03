#ifndef CATEGORY_H
#define CATEGORY_H

#include <QString>

class Category
{
public:
    Category();
    virtual ~Category();

    int id() const;
    void setId(const int &value);

    QString name() const;
    void setName(const QString &newName);

private:
    int m_id;
    QString m_name;
};

#endif // CATEGORY_H
