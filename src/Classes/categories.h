#ifndef CATEGORIES_H
#define CATEGORIES_H

#include <QObject>
#include <QString>

class Category;

class Categories : public QObject
{
    Q_OBJECT
public:
    explicit Categories(QObject *parent = nullptr);
    virtual ~Categories();

    QVector< Category > items() const;
    bool setItemAt(int index, Category &item);
    void addItem(const int &idCategory, const QString &name);

    int rowCount() const;

    int curCategoryId() const;
    void setCurCategoryId(const int &value);

    bool find(const int &id);
    bool find(const QString &name);
    void add(const QString &name);
    int getId(const QString &name);

signals:
    void beforeItemAppended();
    void afterItemAppended();

    void selectItem(int index);

public slots:

private:
    QVector< Category > m_categories;

    int m_curCategoryId {};
};

#endif // CATEGORIES_H
