#ifndef PLFILE_H
#define PLFILE_H

#include <QString>

class PlFile
{
public:
    PlFile();
    virtual ~PlFile();

    int idFile() const;
    void setIdFile(int *idPlaylistFile);

    int idPlaylist() const;
    void setIdPlaylist(int *idPlaylist);

    QString fileName() const;
    void setFileName(const QString *fileName);

    QString filePath() const;
    void setFilePath(const QString *filePath);

    QString filePathLocal() const;
    void setFilePathLocal(const QString *filePathLocal);

    int idFormat() const;
    void setIdFormat(int *idFormat);

    bool isAvailable() const;
    void setIsAvailable(bool *isAvailable);

    QString format() const;
    void setFormat(const QString *format);

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
