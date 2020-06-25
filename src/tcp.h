#ifndef TCP_H_
#define TCP_H_

#include "nobject.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

#define lastError WSAGetLastError() 

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define lastError errno 
#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define SD_SEND SHUT_WR
#define closesocket(descriptor) ::close(descriptor)

#endif

class network
{
#ifdef _WIN32
protected:
    network()
    {
        if (i == 0)
        {
            WSADATA wsaData;
            code = WSAStartup(MAKEWORD(2, 2), &wsaData);
            if (code != 0)
                console::error("WSAStartup failed with error: ", code);
        }
        i++;
    }

    ~network()
    {
        i--;
        if (i == 0)
            WSACleanup();
    }

private:
    static inline int code = 0, i = 0;
#endif
};

class tcpClient : public network
{
public:
    tcpClient() = default;
    void connect(const std::string &adress, unsigned short port)
    {
        struct addrinfo *result = nullptr,
                        *ptr = nullptr,
                        hints;

        std::memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        code = getaddrinfo(adress.c_str(), std::to_string(port).c_str(), &hints, &result);
        if (code != 0)
            throw std::runtime_error("getaddrinfo failed with error: " + std::to_string(code));

        for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
        {

            clientSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

            if (clientSocket == INVALID_SOCKET)
            {
                code = lastError;
                throw std::runtime_error("socket failed with error: " + std::to_string(code));
            }

            code = ::connect(clientSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
            if (code == SOCKET_ERROR)
            {
                closesocket(clientSocket);
                clientSocket = INVALID_SOCKET;
                continue;
            }
            break;
        }

        freeaddrinfo(result);

        if (clientSocket == INVALID_SOCKET)
            throw std::runtime_error("unable to connect to server");
    }

    void send(const std::string &message)
    {
        code = ::send(clientSocket, message.c_str(), message.size(), 0);
        if (code == SOCKET_ERROR)
        {
            code = lastError;
            closesocket(clientSocket);
            throw std::runtime_error("send failed with error: " + std::to_string(code));
        }

        //console::log("Bytes Sent: ", code);
    }

    std::string receive()
    {
        std::string buf(8192, 0);
        code = recv(clientSocket, buf.data(), buf.size(), 0);
        if (code >= 0)
        {
            //console::log("Bytes received: ", code);
            buf.resize(code);
        }
        else
            throw std::runtime_error("recv failed with error: " + std::to_string(code));
        return buf;
    }

    void receive(std::string& buf)
    {
        buf.resize(8192, 0);
        code = recv(clientSocket, buf.data(), buf.size(), 0);
        if (code >= 0)
        {
            //console::log("Bytes received: ", code);
            buf.resize(code);
        }
        else
            throw std::runtime_error("recv failed with error: " + std::to_string(code));
    }

    void close()
    {
        code = shutdown(clientSocket, SD_SEND);
        if (code == SOCKET_ERROR)
        {
            code = lastError;
            closesocket(clientSocket);
        }
        std::string buf(4096, 0);
        do
        {
            code = recv(clientSocket, buf.data(), buf.size(), 0);
        } while (code > 0);
        clientSocket = INVALID_SOCKET;
    }

    ~tcpClient()
    {
        if (clientSocket != INVALID_SOCKET)
            close();
    }

private:
    tcpClient(SOCKET socket) : clientSocket(socket) {}
    friend class tcpServer;
    SOCKET clientSocket = INVALID_SOCKET;
    int code;
};

class tcpServer : public network
{
public:
    void bind(unsigned short port)
    {
        struct addrinfo *result = nullptr,
                        hints;

        std::memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        code = getaddrinfo(nullptr, std::to_string(port).c_str(), &hints, &result);
        if (code != 0)
            throw std::runtime_error("getaddrinfo failed with error: " + std::to_string(code));

        serverSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (serverSocket == INVALID_SOCKET)
        {
            code = lastError;
            freeaddrinfo(result);
            throw std::runtime_error("socket failed with error: " + std::to_string(code));
        }

        //console::log(result->ai_addr->sa_data);

        code = ::bind(serverSocket, result->ai_addr, (int)result->ai_addrlen);
        if (code == SOCKET_ERROR)
        {
            code = lastError;
            freeaddrinfo(result);
            closesocket(serverSocket);
            throw std::runtime_error("bind failed with error: " + std::to_string(code));
        }

        freeaddrinfo(result);

        code = ::listen(serverSocket, SOMAXCONN);
        if (code == SOCKET_ERROR)
        {
            code = lastError;
            closesocket(serverSocket);
            throw std::runtime_error("listen failed with error: " + std::to_string(code));
        }
    }

    std::shared_ptr<tcpClient> listen()
    {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET)
        {
            code = lastError;
            closesocket(serverSocket);
            throw std::runtime_error("accept failed with error: " + std::to_string(code));
        }

        return std::shared_ptr<tcpClient>(new tcpClient(clientSocket));
    }

    void close()
    {
        closesocket(serverSocket);
        serverSocket = INVALID_SOCKET;
    }

    ~tcpServer()
    {
        if (serverSocket != INVALID_SOCKET)
            close();
    }

private:
    SOCKET serverSocket = INVALID_SOCKET;
    int code;
};

#endif /* !TCP_H_ */
