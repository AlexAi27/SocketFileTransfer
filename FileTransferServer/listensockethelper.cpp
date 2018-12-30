#include "listensockethelper.h"
#include <QDebug>
SOCKET ListenSocketHelper::ListenSocket = INVALID_SOCKET;
char ListenSocketHelper::DEFAULT_PORT[6] = "6666";
int ListenSocketHelper::DEFAULT_BUFLEN = 4096;
QString ListenSocketHelper::path = "";

ListenSocketHelper::ListenSocketHelper(int buflen)
{
    setBuflen(buflen);
}

ListenSocketHelper::~ListenSocketHelper() {
    closeSocket();
}

void ListenSocketHelper::setBuflen(int buflen) {
    ListenSocketHelper::DEFAULT_BUFLEN = buflen;
}

int ListenSocketHelper::initSocket(int &err) {
    WSADATA wsaData;
    int iResult;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        err = iResult;
        throw "WSAStartup failed!";
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        err = iResult;
        WSACleanup();
        throw "getaddrinfo failed!";
    }

    // Create a SOCKET for connecting to server
    ListenSocketHelper::ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocketHelper::ListenSocket == INVALID_SOCKET) {
        WSACleanup();
        freeaddrinfo(result);
        err = WSAGetLastError();
        throw "socket failed!";
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocketHelper::ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        err = WSAGetLastError();
        freeaddrinfo(result);
        closesocket(ListenSocketHelper::ListenSocket);
        WSACleanup();
        throw "bind failed!";
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocketHelper::ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        err = WSAGetLastError();
        closesocket(ListenSocketHelper::ListenSocket);
        WSACleanup();
        throw "listen failed!";
    }

    return 0;
}

void ListenSocketHelper::closeSocket() {
    closesocket(ListenSocketHelper::ListenSocket);
    WSACleanup();
}

SOCKET ListenSocketHelper::listenTo(int &err) {
    SOCKET ClientSocket = INVALID_SOCKET;
    ClientSocket = accept(ListenSocketHelper::ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        err = WSAGetLastError();
        closesocket(ListenSocket);
        WSACleanup();
        throw "accept failed!";
    }

    return ClientSocket;
}

int ListenSocketHelper::recvFile(SOCKET ClientSocket, int &err) {
    int iResult, iSendResult;
    char *recvbuf = (char *) malloc(sizeof(char) * ListenSocketHelper::DEFAULT_BUFLEN);
    QString filename;

    iResult = recv(ClientSocket, recvbuf, ListenSocketHelper::DEFAULT_BUFLEN, 0), iSendResult;
    if (iResult > 0) {
        recvbuf[iResult] = '\0';
        filename = recvbuf;

        // Echo the buffer back to the sender
        iSendResult = send( ClientSocket, "1", 1, 0 );
        if (iSendResult == SOCKET_ERROR) {
            err = WSAGetLastError();
            closesocket(ClientSocket);
            WSACleanup();
            free(recvbuf);
            throw "send failed!";
        }
    } else if (iResult < 0) {
        err = WSAGetLastError();
        closesocket(ClientSocket);
        WSACleanup();
        free(recvbuf);
        throw "receive failed!";
    } else {
        qDebug() << "xxx";
        closesocket(ClientSocket);
        err = -1;
        ClientSocket = INVALID_SOCKET;
        throw "Connection closed";
    }
    QString path = ListenSocketHelper::path + "/" + filename;
    qDebug() << path;
    FILE *file = fopen(path.toLocal8Bit().data(), "wb");


    while (true) {
        iResult = recv(ClientSocket, recvbuf, ListenSocketHelper::DEFAULT_BUFLEN, 0);
        qDebug() << iResult;
        if (iResult > 0) {
            fwrite(recvbuf, sizeof(char), iResult, file);

        // Echo the buffer back to the sender
            iSendResult = send( ClientSocket, "1", 1, 0 );
            if (iSendResult == SOCKET_ERROR) {
                err = WSAGetLastError();
                closesocket(ClientSocket);
                WSACleanup();
                fclose(file);
                free(recvbuf);
                throw "send failed!";
            }
        }
        else if (iResult == 0){
            closesocket(ClientSocket);
            break;
        }
        else  {
            err = WSAGetLastError();
            closesocket(ClientSocket);
            WSACleanup();
            fclose(file);
            free(recvbuf);
            throw "receive failed!";
        }
    }
    fclose(file);
    free(recvbuf);
    err = 0;
    return 0;
}
