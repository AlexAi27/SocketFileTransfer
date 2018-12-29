#ifndef RECVTHREAD_H
#define RECVTHREAD_H

#include <QThread>
#include "listensockethelper.h"

class RecvThread : public QThread
{
public:
    RecvThread(SOCKET clientSocket, QString filename);
    void run();

private:
    SOCKET ClientSocket;
    QString fileName;
};

#endif // RECVTHREAD_H
