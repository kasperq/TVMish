#ifndef EPGS_H
#define EPGS_H

#include <QObject>
#include <QUrl>

class Epg;

class Epgs : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int currentIdEpg READ curIdEpg)
    Q_PROPERTY(QUrl currentUrl READ curUrl)
public:
    explicit Epgs(QObject *parent = nullptr);
    virtual ~Epgs();

    QVector< Epg > items() const;
    bool setItemAt(int index, Epg &item);
    void addItem(const int &idEpg, const QString &naim, const QUrl &url, const QDateTime &dateTimeUpdate,
                 const QDateTime &dateTimeBegin, const QDateTime &dateTimeEnd);
    void clear();

    int rowCount() const;
    void initConnections();

    int curIdEpg() const;

    const QUrl &curUrl() const;

signals:
    void itemEdited(const int &index, const int &idEpg, const QString &naim, const QUrl &url,
                    const QDateTime &dateTimeUpdate, const QDateTime &dateTimeBegin, const QDateTime &dateTimeEnd);
    void itemAppended(const int &index, const QString &naim, const QUrl &url,
                      const QDateTime &dateTimeUpdate, const QDateTime &dateTimeBegin, const QDateTime &dateTimeEnd);
    void epgDeleted(const int &index, const int &idEpg);

    void onCurIdEpgChanged();
    void onCurUrlChanged();

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
    void setNewIdEpg(const int &index, const int &newId);

    void appendNewItem();
    void update(const int &index, const QUrl &fileUrl);
    void deleteEpg(const int &index, const int &idEpg);

private:
    QVector< Epg > m_epgs;

    int m_curIdEpg {};
    QUrl m_curUrl;

};

#endif // EPGS_H
