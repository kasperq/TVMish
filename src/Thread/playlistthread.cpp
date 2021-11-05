#include "playlistthread.h"

PlaylistThread::PlaylistThread()
{

}

void PlaylistThread::run()
{
    m_oldFile.setFileName(m_oldPath);
    m_oldFile.copy(m_newPath);
}

void PlaylistThread::setOldNewPaths(QString &oldPath, QString &newPath)
{
    m_oldPath = oldPath;
    m_newPath = newPath;
}
