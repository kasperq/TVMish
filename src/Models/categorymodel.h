#ifndef CATEGORYMODEL_H
#define CATEGORYMODEL_H

#include <QAbstractListModel>
#include <QObject>

#include "./Classes/categories.h"
#include "./Classes/category.h"

class CategoryModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Categories *list READ categories WRITE setCategories /*NOTIFY categoriesChanged*/)
public:
    explicit CategoryModel(QObject *parent = nullptr);
    virtual ~CategoryModel();

    Categories *categories() const;
    void setCategories(Categories *newCategories);

    enum {
        IdCategoryRole = Qt::UserRole,
        NaimRole,
        IsReadOnlyRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

//signals:
//    void categoriesChanged();

private:
    Categories *m_categories;

};

#endif // CATEGORYMODEL_H
