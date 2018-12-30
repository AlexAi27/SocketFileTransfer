#include "listenthread.h"
#include "recvthread.h"
#include <QMessageBox>

ListenThread::ListenThread()
{

}

void ListenThread::run() {
    int err;
    QString name;
    while (true) {
        try {
            SOCKET clientSocket = ListenSocketHelper::listenTo(err);
            if (clientSocket == INVALID_SOCKET) continue;
            RecvThread *recvThread = new RecvThread(clientSocket);
            recvThread->start();
        } catch (const char *msg) {
            QMessageBox::warning(NULL, "Error", msg, QMessageBox::Ok);
        }
    }
}
