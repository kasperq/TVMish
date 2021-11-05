#ifndef PLAYLISTTHREAD_H
#define PLAYLISTTHREAD_H

#include <QThread>
#include <QObject>
#include <QFile>

class PlaylistThread : public QThread
{
    Q_OBJECT
public:
    PlaylistThread();

    void run() override;

    void setOldNewPaths(QString &oldPath, QString &newPath);

signals:
    void error();

private:
    QFile m_oldFile;
    QString m_oldPath;
    QString m_newPath;
};

#endif // PLAYLISTTHREAD_H
