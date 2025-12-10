#ifndef EPGMODEL_H
#define EPGMODEL_H

#include <QAbstractListModel>
#include <QObject>

#include "./Classes/epgs.h"

class EpgsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Epgs *list READ epgs WRITE setEpgs NOTIFY onEpgsChanged)
public:
    explicit EpgsModel(QObject *parent = nullptr);
    virtual ~EpgsModel();

    enum {
        IdEpgRole = Qt::UserRole,
        NaimRole,
        UrlRole,
        DateTimeUpdateRole,
        DateTimeBeginRole,
        DateTimeEndRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    Epgs *epgs() const;
    void setEpgs(Epgs *newEpgs);

signals:
    void onEpgsChanged();

public slots:

private:
    Epgs *m_epgs;
};

#endif // EPGMODEL_H
