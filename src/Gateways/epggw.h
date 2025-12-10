#ifndef EPGGW_H
#define EPGGW_H

#include <QObject>
#include <QSqlQuery>

class EpgGW : public QObject
{
    Q_OBJECT
public:
    explicit EpgGW(QObject *parent = nullptr);
    virtual ~EpgGW();

    QSqlQuery *data() const;
    int rows() const;

signals:
    void selected();
    void inserted(const int &index, const int &newId);
    void modified(const int &index);
    void deleted(const int &index, const int &id);

public slots:
    void select();
    void insert(const int &index, const QString &naim, const QUrl &url, const QDateTime &dateTimeUpdate,
                const QDateTime &dateTimeBegin, const QDateTime &dateTimeEnd);
    void modify(const int &index, const int &idEpg, const QString &naim, const QUrl &url,
                const QDateTime &dateTimeUpdate, const QDateTime &dateTimeBegin, const QDateTime &dateTimeEnd);
    void deleteRecord(const int &index, const int &idEpg);
    void save();

private:
    QSqlQuery q_query;
    QSqlQuery q_temp;

    int m_rows {};
    QString m_queryStr {};

    int m_newId {};

    void calcRowCount();
    int getMaxId();
};

#endif // EPGGW_H
