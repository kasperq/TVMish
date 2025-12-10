#ifndef EPGKART_H
#define EPGKART_H

#include <QString>
#include <QUrl>
#include <QDateTime>

class EpgKart
{
public:
    EpgKart();
    virtual ~EpgKart();

    int idKart() const;
    void setIdKart(const int &newIdKart);

    int idEpg() const;
    void setIdEpg(const int &newIdEpg);

    int idChannel() const;
    void setIdChannel(const int &newIdChannel);

    int epgCode() const;
    void setEpgCode(const int &newEpgCOde);

    const QDateTime &timeStart() const;
    void setTimeStart(const QDateTime &newTimeStart);

    const QDateTime &timeEnd() const;
    void setTimeEnd(const QDateTime &newTimeEnd);

    int timeShift() const;
    void setTimeShift(const int &newTimeShift);

    const QString &title() const;
    void setTitle(const QString &newTitle);

    const QString &description() const;
    void setDescription(const QString &newDescription);

    const QUrl &iconUrl() const;
    void setIconUrl(const QUrl &newIconUrl);

    int idLogo() const;
    void setIdLogo(const int &newIdLogo);

    const QString &category() const;
    void setCategory(const QString &newCategory);

    const QString &ageRating() const;
    void setAgeRating(const QString &newAgeRating);

private:
    int m_idKart {};
    int m_idEpg {};
    int m_idChannel {};
    int m_epgCode {};
    QDateTime m_timeStart {};
    QDateTime m_timeEnd {};
    int m_timeShift {};
    QString m_title {};
    QString m_description {};
    QUrl m_iconUrl {};
    int m_idLogo {};
    QString m_category {};
    QString m_ageRating {};

};

#endif // EPGKART_H
