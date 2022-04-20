#ifndef CATEGORY_H
#define CATEGORY_H

#include <QString>

class Category
{
public:
    Category();
    virtual ~Category();

    int idCategory() const;
    void setIdCategory(const int &value);

    QString name() const;
    void setName(const QString &newName);

    bool isReadonly() const;
    void setIsReadonly(const bool &newIsReadonly);

private:
    int m_idCategory {};
    QString m_name {};
    bool m_isReadonly {false};
};

#endif // CATEGORY_H
