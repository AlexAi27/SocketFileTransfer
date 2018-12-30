#ifndef LISTENSOCKETHELPER_H
#define LISTENSOCKETHELPER_H

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

class ListenSocketHelper
{
public:
    ListenSocketHelper(int buflen = 4096);
    ~ListenSocketHelper();
    void setBuflen(int buflen);
    int initSocket(int &err);
    static SOCKET listenTo(int &err);
    static int recvFile(SOCKET ClientSocket, int &err);
    void closeSocket();

    static SOCKET ListenSocket;
    static char DEFAULT_PORT[6];
    static int DEFAULT_BUFLEN;
    static QString path;
};

#endif // LISTENSOCKETHELPER_H
