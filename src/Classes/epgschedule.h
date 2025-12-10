#ifndef EPGSCHEDULE_H
#define EPGSCHEDULE_H

#include <QObject>

#include "epgkart.h"

class EpgSchedule : public QObject
{
    Q_OBJECT
public:
    explicit EpgSchedule(QObject *parent = nullptr);
    virtual ~EpgSchedule();

    QVector< EpgKart > items() const;
    bool setItemAt(int index, EpgKart &item);
    void addItem(const int &idKart, const int &idEpg, const int &idChannel, const int &epgCode, const QDateTime &timeStart,
                 const QDateTime &timeEnd, const int &timeShift, const QString &title, const QString &description,
                 const QUrl &iconUrl, const int &idLogo, const QString &category, const QString &ageRating);
    void clear();

    int rowCount() const;
    void initConnections();

    int curIdEpgKart() const;

signals:
    void itemEdited(const int &index, const int &idKart, const int &idEpg, const int &idChannel, const int &epgCode,
                    const QDateTime &timeStart, const QDateTime &timeEnd, const int &timeShift, const QString &title,
                    const QString &description, const QUrl &iconUrl, const int &idLogo, const QString &category,
                    const QString &ageRating);
    void itemAppended(const int &index, const int &idKart, const int &idEpg, const int &idChannel, const int &epgCode,
                      const QDateTime &timeStart, const QDateTime &timeEnd, const int &timeShift, const QString &title,
                      const QString &description, const QUrl &iconUrl, const int &idLogo, const QString &category,
                      const QString &ageRating);
    void epgDeleted(const int &index, const int &idEpg);

    void onCurIdKartChanged();

    void rowCountChanged(int rows);

    void beforeItemAppended();
    void afterItemAppended();

    void beforeItemRemoved(int index);
    void afterItemRemoved();

    void selectItem(int index);
    void itemChanged(int index);

public slots:
    void open();
    void scroll(const int &index);
    void setNewIdKart(const int &index, const int &newId);

    void appendNewItem();

private:
    QVector< EpgKart > m_kart;

    int m_curIdKart {};

};

#endif // EPGSCHEDULE_H
