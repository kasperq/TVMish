#include "poolthread.h"

#include <QDebug>
PoolThread::PoolThread()
{

}

PoolThread::~PoolThread()
{

}

void PoolThread::run()
{
    qDebug() << "PoolThread is running";
    m_isRunning = true;
    emit isStarted();
}

void PoolThread::setIsRunning(const bool &newIsRunning)
{
    m_isRunning = newIsRunning;
}

bool PoolThread::isRunning() const
{
    return m_isRunning;
}
