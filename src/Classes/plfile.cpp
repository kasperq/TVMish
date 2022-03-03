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

void PlFile::setIdFile(const int &value)
{
    m_idFile = value;
}

int PlFile::idPlaylist() const
{
    return m_idPlaylist;
}

void PlFile::setIdPlaylist(const int &value)
{    
    m_idPlaylist = value;
}

QString PlFile::fileName() const
{
    return m_fileName;
}

void PlFile::setFileName(const QString &value)
{
    m_fileName = value;
}

QString PlFile::filePath() const
{
    return m_filePath;
}

void PlFile::setFilePath(const QString &value)
{
    m_filePath = value;
}

QString PlFile::filePathLocal() const
{
    return m_filePathLocal;
}

void PlFile::setFilePathLocal(const QString &value)
{
    m_filePathLocal = value;
}

int PlFile::idFormat() const
{
    return m_idFormat;
}

void PlFile::setIdFormat(const int &value)
{
    m_idFormat = value;
}

bool PlFile::isAvailable() const
{
    return m_isAvailable;
}

void PlFile::setIsAvailable(const bool &value)
{
    m_isAvailable = value;
}

QString PlFile::format() const
{
    return m_format;
}

void PlFile::setFormat(const QString &value)
{
    m_format = value;
}
