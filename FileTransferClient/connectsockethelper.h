#ifndef CONNECTSOCKETHELPER_H
#define CONNECTSOCKETHELPER_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define _WIN32_WINNT  0x501

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include <QString>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class ConnectSocketHelper
{
public:
    ConnectSocketHelper(int buflen = 4096);
    ConnectSocketHelper(QString ipaddr, QString port, int &err, int buflen = 4096);
    ~ConnectSocketHelper();
    void setBuflen(int buflen);
    int initSocket(QString ipaddr, QString port, int &err);
    int transferFile(QString path);
    void closeSocket();

    static SOCKET ConnectSocket;
    static char DEFAULT_PORT[6];
    static int DEFAULT_BUFLEN;
};


#endif // CONNECTSOCKETHELPER_H
