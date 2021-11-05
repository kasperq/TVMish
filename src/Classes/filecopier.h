#ifndef FILECOPIER_H
#define FILECOPIER_H

#include <QThread>

class FileCopier : public QThread
{
    Q_OBJECT
public:
    FileCopier();
};

#endif // FILECOPIER_H
