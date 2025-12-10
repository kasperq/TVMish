#ifndef EPG_H
#define EPG_H

#include <QString>
#include <QDateTime>
#include <QUrl>

class Epg
{
public:
    Epg();
    virtual ~Epg();

    int idEpg() const;
    void setIdEpg(const int &newIdEpg);

    const QUrl &url() const;
    void setUrl(const QUrl &newUrl);

    const QString &naim() const;
    void setNaim(const QString &newNaim);

    const QDateTime &dateTimeUpdate() const;
    void setDateTimeUpdate(const QDateTime &newDateTimeUpdate);

    const QDateTime &dateTimeEnd() const;
    void setDateTimeEnd(const QDateTime &newDateTimeEnd);

    const QDateTime &dateTimeBegin() const;
    void setDateTimeBegin(const QDateTime &newDateTimeBegin);

private:
    int m_idEpg {};
    QUrl m_url {};
    QString m_naim {};
    QDateTime m_dateTimeUpdate {};
    QDateTime m_dateTimeBegin {};
    QDateTime m_dateTimeEnd {};
};

#endif // EPG_H
