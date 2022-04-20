#ifndef PLFILEMODEL_H
#define PLFILEMODEL_H

#include <QAbstractListModel>

#include "./Classes/plfiles.h"

class PlFilesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(PlFiles *list READ files WRITE setFiles)
    Q_PROPERTY(int rowCount READ rowCount)
public:
    PlFilesModel();
    virtual ~PlFilesModel();

    enum {
        FileNameRole = Qt::UserRole,
        FilePathRole,
        FilePathLocalRole,
        IdPlaylistRole,
        IdFileRole,
        IdFormatRole,
        IsAvailableRole,
        FormatRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QHash<int, QByteArray> roleNames() const override;


    PlFiles *files() const;
    void setFiles(PlFiles *files);

private:
    PlFiles *m_files;
};

#endif // PLFILEMODEL_H
