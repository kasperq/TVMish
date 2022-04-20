#ifndef POOLTHREAD_H
#define POOLTHREAD_H

#include <QRunnable>
#include <QObject>

class PoolThread : public QObject, public QRunnable
{
    Q_OBJECT
public:
    PoolThread();
    virtual ~PoolThread();
    void run() override;

    void setIsRunning(const bool &newIsRunning);
    bool isRunning() const;

signals:
    void isStarted();
private:
    bool m_isRunning {false};

};

#endif // POOLTHREAD_H
