#ifndef CHANNEL_H
#define CHANNEL_H

#include <QString>

class Channel
{
public:
    Channel();
    virtual ~Channel();

    void clear();

    int idChannel() const;
    void setIdChannel(const int &newIdChannel);

    QString naim() const;
    void setNaim(const QString &newNaim);

    QString url() const;
    void setUrl(const QString &newUrl);

    int idFile() const;
    void setIdFile(const int &newIdFile);

    int idPlaylist() const;
    void setIdPlaylist(const int &newIdPlaylist);

    int idCategory() const;
    void setIdCategory(const int &newIdCategory);

    int numFile() const;
    void setNumFile(const int &newNumFile);

    int numPlaylist() const;
    void setNumPlaylist(const int &newNumPlaylist);

    int archDays() const;
    void setArchDays(const int &newArchDays);

    int idLogo() const;
    void setIdLogo(const int &newLogo);

private:
    int m_idChannel {};
    QString m_naim;
    QString m_url;
    int m_idFile {};
    int m_idPlaylist {};
    int m_idCategory {};
    int m_numFile {};
    int m_numPlaylist {};
    int m_archDays {};
    int m_idLogo {};

};

#endif // CHANNEL_H
