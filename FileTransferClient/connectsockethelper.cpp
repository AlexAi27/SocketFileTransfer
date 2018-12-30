#include "connectsockethelper.h"
#include <QDebug>

SOCKET ConnectSocketHelper::ConnectSocket = INVALID_SOCKET;
char ConnectSocketHelper::DEFAULT_PORT[6] = "6666";
int ConnectSocketHelper::DEFAULT_BUFLEN = 4096;

ConnectSocketHelper::ConnectSocketHelper(int buflen){
    setBuflen(buflen);
}

ConnectSocketHelper::ConnectSocketHelper(QString ipaddr, QString port, int &err, int buflen){
    setBuflen(buflen);
    initSocket(ipaddr, port, err);
}

ConnectSocketHelper::~ConnectSocketHelper() {
    closeSocket();
}

void ConnectSocketHelper::setBuflen(int buflen) {
    ConnectSocketHelper::DEFAULT_BUFLEN = buflen;
}

int ConnectSocketHelper::initSocket(QString ipaddr, QString port, int &err) {
    strcpy(ConnectSocketHelper::DEFAULT_PORT, port.toUtf8().data());
    WSADATA wsaData;
    ConnectSocketHelper::ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
            *ptr = NULL,
            hints;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        err = iResult;
        throw "WSAStartup failed!";
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(ipaddr.toUtf8().data(), DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        err = iResult;
        WSACleanup();
        throw "getaddrinfo failed!";
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocketHelper::ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocketHelper::ConnectSocket == INVALID_SOCKET) {
            WSACleanup();
            err = WSAGetLastError();
            throw "socket failed!";
        }

        // Connect to server.
        iResult = connect( ConnectSocketHelper::ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocketHelper::ConnectSocket);
            ConnectSocketHelper::ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }
    freeaddrinfo(result);

    if (ConnectSocketHelper::ConnectSocket == INVALID_SOCKET) {
        WSACleanup();
        err = -1;
        throw "Unable to connect to server!";
    }

    err = 0;
    return 0;
}

void ConnectSocketHelper::closeSocket() {
    shutdown(ConnectSocket, SD_SEND);
    closesocket(ConnectSocketHelper::ConnectSocket);
    WSACleanup();
}

int ConnectSocketHelper::transferFile(QString path) {
    // qDebug() << path;
    FILE *file = fopen(path.toLocal8Bit().data(), "rb");
    int iResult;
    char *recvbuf = (char *) malloc(sizeof(char) * ConnectSocketHelper::DEFAULT_BUFLEN);
    char *sendbuf = (char *) malloc(sizeof(char) * ConnectSocketHelper::DEFAULT_BUFLEN);

    int size;
    fseek(file, 0L, FILE_END);
    size = ftell(file);
    rewind(file);
    // qDebug() << size;
    char *tmp = (char *) malloc(sizeof(char) * 4096);

    strcpy(tmp, path.toUtf8().data());
    strcpy(sendbuf, strrchr(tmp, '/')+1);
    // qDebug() << sendbuf;
    iResult = send(ConnectSocketHelper::ConnectSocket, sendbuf, strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        QString err = "Sending failed with error code : ";
        err += QString::number(WSAGetLastError());
        closesocket(ConnectSocketHelper::ConnectSocket);
        WSACleanup();
        free(sendbuf);
        free(recvbuf);
        fclose(file);
        throw err.toLocal8Bit().data();
    }

    iResult = recv(ConnectSocketHelper::ConnectSocket, recvbuf, ConnectSocketHelper::DEFAULT_BUFLEN, 0);
    if (iResult == 0) {
        free(sendbuf);
        free(recvbuf);
        fclose(file);
        throw "Connection closed!";
    } else if (iResult < 0){
        QString err = "Receiving failed with error code : ";
        err += QString::number(WSAGetLastError());
        free(sendbuf);
        free(recvbuf);
        fclose(file);
        throw err.toLocal8Bit().data();
    }

    int pre = 0, buflen = ConnectSocketHelper::DEFAULT_BUFLEN, delta = 0;
    while (true) {
        fread(sendbuf, sizeof(char), buflen, file);
        delta = buflen;
        pre += delta;
        if (pre > size) delta -= pre - size;
        int iResult = send(ConnectSocketHelper::ConnectSocket, sendbuf, delta, 0);
        if (iResult == SOCKET_ERROR) {
            QString err = "Sending failed with error code : ";
            err += QString::number(WSAGetLastError());
            closesocket(ConnectSocketHelper::ConnectSocket);
            WSACleanup();
            free(sendbuf);
            free(recvbuf);
            fclose(file);
            throw err.toLocal8Bit().data();
        }

        iResult = recv(ConnectSocketHelper::ConnectSocket, recvbuf, ConnectSocketHelper::DEFAULT_BUFLEN, 0);
        if (iResult == 0) {
            free(sendbuf);
            free(recvbuf);
            fclose(file);
            throw "Connection closed!";
        } else if (iResult < 0){
            QString err = "Receiving failed with error code : ";
            err += QString::number(WSAGetLastError());
            free(sendbuf);
            free(recvbuf);
            fclose(file);
            throw err.toLocal8Bit().data();
        }
        if (pre >= size) {
            break;
        }
    }
    shutdown(ConnectSocketHelper::ConnectSocket, SD_SEND);
    free(sendbuf);
    free(recvbuf);
    fclose(file);
    return 0;
}
