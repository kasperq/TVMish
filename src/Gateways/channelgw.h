#ifndef CHANNELGW_H
#define CHANNELGW_H

#include <QObject>
#include <QSqlQuery>

class ChannelGW : public QObject
{
    Q_OBJECT
public:
    explicit ChannelGW(QObject *parent = nullptr);
    virtual ~ChannelGW();

    QSqlQuery *data() const;
    int rows() const;

signals:
    void selected(const int &idPlaylist, const int &idFile);
    void inserted(const int &index, const int &newId, const int &numPlaylist, const int &idFile, const int &idPlaylist);
    void modified(const int &index);
    void deleted(const int &index, const int &id);
    void deletedAllInFile(const int &idFile);
    void deletedAllInPlaylist(const int &index, const int &idPlaylist);

    void numsRecieved(const int &index, const int &idPlaylist, const int &idFile,const int &maxNumPlaylist);
    void sendFavoriteResult(const int &index, const int &idChannel, const int& num);

public slots:
    void select(const int &idPlaylist, const int &idFile);
    void selectWithFilter(const int &idPlaylist, const int &idFile, const QString &filterNaim = "", const int &idCategory = 0);
    void insert(const int &index, const QString &naim, const QString &url, const int &idFile, const int &idPlaylist,
                const int &idCategory, const int &numPlaylist, const int &archDays,
                const int &idLogo, const bool &generateNums, const bool &isFavorite, const int &numFav, const bool &isAvailable);
    void modify(const int &index, const int &idChannel, const QString &naim, const QString &url, const int &idFile,
                const int &idPlaylist, const int &idCategory, const int &numPlaylist, const int &archDays, const int &idLogo,
                const bool &isFavorite, const int &numFav, const bool &isAvailable);
    void setFavorite(const int &index, const bool &isFavorite, const int &idChannel, const int &idPlaylist);
    void deleteRecord(const int &index, const int &idChannel);
    void deleteAllInFile(const int &index, const int &idFile);
    void deleteAllInPlaylist(const int &index, const int &idPlaylist);
    void getNums(const int &index, const int &idPlaylist, const int &idFile);
    int getNumPlaylist(const int &idPlaylist);
    void save();

    void prepareInsert();
//    void addForInsert(const int &index, const QString &naim, const QString &url, const int &idFile, const int &idPlaylist,
//                      const int &idCategory, const int &numFile, const int &numPlaylist, const int &archDays,
//                      const int &idLogo, const bool &generateNums);
//    void saveInsert();

private:
    QSqlQuery q_select;
    QSqlQuery q_modify;
    QSqlQuery q_insert;
    QSqlQuery q_delete;
    QSqlQuery q_temp;

    int m_rows {};
    QString m_insertText {};
    bool m_isPreparedForInsert {false};
    int m_newId {};
    int m_maxNumPlaylist {};
    QString m_filterNaim {};
    int m_idCategory {};

    void calcRowCount();
    int getMaxId();
    void getMaxs();
    int getMaxNumFavorite(const int &idPlaylist);

};

#endif // CHANNELGW_H
