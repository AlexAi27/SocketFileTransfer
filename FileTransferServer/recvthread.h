#ifndef RECVTHREAD_H
#define RECVTHREAD_H

#include <QThread>
#include "listensockethelper.h"

class RecvThread : public QThread
{
public:
    RecvThread(SOCKET clientSocket);
    void run();

private:
    SOCKET ClientSocket;
};

#endif // RECVTHREAD_H
