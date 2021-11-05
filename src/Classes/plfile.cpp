#include "plfile.h"

PlFile::PlFile()
{

}

PlFile::~PlFile()
{

}

int PlFile::idFile() const
{
    return m_idFile;
}

void PlFile::setIdFile(int *idPlaylistFile)
{
    m_idFile = *idPlaylistFile;
}

int PlFile::idPlaylist() const
{
    return m_idPlaylist;
}

void PlFile::setIdPlaylist(int *idPlaylist)
{
    m_idPlaylist = *idPlaylist;
}

QString PlFile::fileName() const
{
    return m_fileName;
}

void PlFile::setFileName(const QString *fileName)
{
    m_fileName = *fileName;
}

QString PlFile::filePath() const
{
    return m_filePath;
}

void PlFile::setFilePath(const QString *filePath)
{
    m_filePath = *filePath;
}

QString PlFile::filePathLocal() const
{
    return m_filePathLocal;
}

void PlFile::setFilePathLocal(const QString *filePathLocal)
{
    m_filePathLocal = *filePathLocal;
}

int PlFile::idFormat() const
{
    return m_idFormat;
}

void PlFile::setIdFormat(int *idFormat)
{
    m_idFormat = *idFormat;
}

bool PlFile::isAvailable() const
{
    return m_isAvailable;
}

void PlFile::setIsAvailable(bool *isAvailable)
{
    m_isAvailable = *isAvailable;
}

QString PlFile::format() const
{
    return m_format;
}

void PlFile::setFormat(const QString *format)
{
    m_format = *format;
}
