#include "recvthread.h"

RecvThread::RecvThread(SOCKET clientSocket)
{
    ClientSocket = clientSocket;
}

void RecvThread::run() {
    int err;
    try{
        ListenSocketHelper::recvFile(ClientSocket, err);
    } catch (const char *msg) {

    }
}
