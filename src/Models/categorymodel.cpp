#include "categorymodel.h"

CategoryModel::CategoryModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_categories = new Categories();
}

CategoryModel::~CategoryModel()
{
    m_categories = nullptr;
}

Categories *CategoryModel::categories() const
{
    return m_categories;
}

void CategoryModel::setCategories(Categories *newCategories)
{        
    beginResetModel();

    if (m_categories)
        m_categories->disconnect(this);

    m_categories = newCategories;

    if (m_categories) {
        connect(m_categories, &Categories::beforeItemAppended, this, [=]() {
            const int index = m_categories->items().size();
            beginInsertRows(QModelIndex(), index, index);
        });

        connect(m_categories, &Categories::afterItemAppended, this, [=]() {
            endInsertRows();
        });

//        connect(m_categories, &Categories::beforeItemRemoved, this, [=](int index) {
//            beginRemoveRows(QModelIndex(), index, index);
//        });

//        connect(m_categories, &Categories::afterItemRemoved, this, [=]() {
//            endRemoveRows();
//        });
    }

    endResetModel();
}

int CategoryModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !m_categories)
        return 0;
    return m_categories->items().size();
}

QVariant CategoryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return false;
    int idCategory {};
    QString naim {};
    bool isReadOnly {false};

    const Category cat = m_categories->items().at(index.row());

    switch (role) {
    case IdCategoryRole:
        idCategory = cat.idCategory();
        return QVariant(idCategory);
    case NaimRole:
        naim = cat.name();
        return QVariant(naim);
    case IsReadOnlyRole:
        isReadOnly = cat.isReadonly();
        return QVariant(isReadOnly);
    }
    return QVariant();
}

bool CategoryModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!m_categories)
        return false;

    int idCategory {};
    QString naim {};
    bool isReadOnly {false};

    Category cat = m_categories->items().at(index.row());
    switch (role) {
    case IdCategoryRole:
        idCategory = value.toInt();
        cat.setIdCategory(idCategory);
        break;
    case NaimRole:
        naim = value.toString();
        cat.setName(naim);
        break;
    case IsReadOnlyRole:
        isReadOnly = value.toBool();
        cat.setIsReadonly(isReadOnly);
        break;
    }
    if (m_categories->setItemAt(index.row(), cat)) {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }

    return false;
}

Qt::ItemFlags CategoryModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> CategoryModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[IdCategoryRole] = "id_category";
    names[NaimRole] = "naim";
    names[IsReadOnlyRole] = "is_readOnly";

    return names;
}
