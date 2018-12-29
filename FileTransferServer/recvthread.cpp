#include "recvthread.h"

RecvThread::RecvThread(SOCKET clientSocket, QString filename)
{
    ClientSocket = clientSocket;
    fileName = filename;
}

void RecvThread::run() {
    int err;
    try{
        ListenSocketHelper::recvFile(ClientSocket, fileName, err);
    } catch (const char *msg) {

    }
}
