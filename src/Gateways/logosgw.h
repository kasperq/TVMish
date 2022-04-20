#ifndef LOGOSGW_H
#define LOGOSGW_H

#include <QObject>
#include <QSqlQuery>

class LogosGW : public QObject
{
    Q_OBJECT
public:
    explicit LogosGW(QObject *parent = nullptr);
    virtual ~LogosGW();

    QSqlQuery *data() const;
    int rows() const;

signals:
    void selected(const int &id);
    void inserted(const int &index, const int &newId, const QString &logoPath);
    void modified(const int &index, const int &id);
    void deleted(const int &index, const int &id);
    void found(const int &index, const bool &isFinded, const int &id, const QString &logoPath, const QString &logoPathSmall);

public slots:
    void select(const int &idLogo);
    void insert(const int &index, const QString &channelName, const QString &logoPath, const QString &logoPathSmall,
                const QString &epgCode);
    void modify(const int &index, const int &idLogo, const QString &channelName, const QString &logoPath,
                const QString &logoPathSmall, const QString &epgCode);
    void modifyPath(const int &idLogo, const QString &logoPath, const QString &logoPathSmall);
    void deleteRecord(const int &index, const int &idLogo);
    bool find(const int &index, const QString &channelName);

private:
    QSqlQuery q_select;
    QSqlQuery q_modify;
    QSqlQuery q_insert;
    QSqlQuery q_delete;
    QSqlQuery q_temp;

    int m_rows {};
    int m_newId {};

    void calcRowCount();
    int getMaxId();

};

#endif // LOGOSGW_H
