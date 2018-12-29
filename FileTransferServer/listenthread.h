#ifndef LISTENTHREAD_H
#define LISTENTHREAD_H

#include <QThread>
#include "listensockethelper.h"

class ListenThread : public QThread
{
public:
    ListenThread();
    void run();
};

#endif // LISTENTHREAD_H
