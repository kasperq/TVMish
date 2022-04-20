#ifndef PLFILE_H
#define PLFILE_H

#include <QString>

class PlFile
{
public:
    PlFile();
    virtual ~PlFile();

    int idFile() const;
    void setIdFile(const int &value);

    int idPlaylist() const;
    void setIdPlaylist(const int &value);

    QString fileName() const;
    void setFileName(const QString &value);

    QString filePath() const;
    void setFilePath(const QString &value);

    QString filePathLocal() const;
    void setFilePathLocal(const QString &value);

    int idFormat() const;
    void setIdFormat(const int &value);

    bool isAvailable() const;
    void setIsAvailable(const bool &value);

    QString format() const;
    void setFormat(const QString &value);

private:
    int m_idFile;
    int m_idPlaylist;
    QString m_fileName;
    QString m_filePath;
    QString m_filePathLocal;
    int m_idFormat;
    QString m_format;
    bool m_isAvailable;

};

#endif // PLFILE_H
