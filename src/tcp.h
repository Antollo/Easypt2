#ifndef TCP_H_
#define TCP_H_

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

#define lastError WSAGetLastError()

#include <openssl/applink.c>

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

#define ERROR -1

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

/// Base class for TCP sockets
class tcpSocket
{
public:
    virtual void close()
    {
        closesocket(socket);
        socket = INVALID_SOCKET;
    }

protected:
    tcpSocket(SOCKET newSocket) : tcpSocket()
    {
        socket = newSocket;
    }

    constexpr static size_t bufferSize = 8192;
    SOCKET socket = INVALID_SOCKET;

#ifdef _WIN32
public:
    tcpSocket()
    {
        if (counter == 0)
        {
            WSADATA wsaData;
            int code = WSAStartup(MAKEWORD(2, 2), &wsaData);
            if (code != 0)
                console::error("WSAStartup failed with error: ", code);
        }
        counter++;
    }

    virtual ~tcpSocket()
    {
        counter--;
        if (counter == 0)
        {
            WSACleanup();
        }
        if (socket != INVALID_SOCKET)
            close();
    }

private:
    static inline size_t counter = 0;
#else
public:
    tcpSocket() = default;
    virtual ~tcpSocket()
    {
        if (socket != INVALID_SOCKET)
            close();
    }

#endif
};

/// Base class for SSL connections
class sslSocket
{
protected:
    sslSocket()
    {
        if (!sslInitialized)
        {
            SSL_library_init();
            SSL_load_error_strings();
            OpenSSL_add_all_algorithms();
            sslInitialized = true;
        }
    }

    sslSocket(std::shared_ptr<SSL_CTX> newSslContext, std::shared_ptr<SSL> newSsl) : sslSocket()
    {
        sslContext = newSslContext;
        ssl = newSsl;
    }

    std::string getSslError()
    {
        BIO *bio = BIO_new(BIO_s_mem());
        ERR_print_errors(bio);
        char *buf;
        size_t len = BIO_get_mem_data(bio, &buf);
        std::string ret(buf, len);
        BIO_free(bio);
        return ret;
    }

    static void freeSslContext(SSL_CTX *sslContext)
    {
        SSL_CTX_free(sslContext);
    }

    static void freeSsl(SSL *ssl)
    {
        SSL_shutdown(ssl);
        SSL_free(ssl);
    }

    void close()
    {
        ssl.reset();
        sslContext.reset();
    }

    std::shared_ptr<SSL_CTX> sslContext;
    std::shared_ptr<SSL> ssl;

private:
    static inline bool sslInitialized = false;
};

/// TCP client
class tcpClient : public tcpSocket
{
public:
    tcpClient() : tcpSocket() {}

    virtual void connect(const std::string &adress, unsigned short port)
    {
        struct addrinfo *result = nullptr, *ptr = nullptr, hints;

        std::memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        int code = getaddrinfo(adress.c_str(), std::to_string(port).c_str(), &hints, &result);
        if (code != 0)
            throw std::runtime_error("getaddrinfo failed with error: " + std::to_string(code));

        for (ptr = result; ptr != nullptr; ptr = ptr->ai_next)
        {

            socket = ::socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

            if (socket == INVALID_SOCKET)
            {
                code = lastError;
                throw std::runtime_error("socket failed with error: " + std::to_string(code));
            }

            code = ::connect(socket, ptr->ai_addr, (int)ptr->ai_addrlen);
            if (code == SOCKET_ERROR)
            {
                closesocket(socket);
                socket = INVALID_SOCKET;
                continue;
            }
            break;
        }

        freeaddrinfo(result);

        if (socket == INVALID_SOCKET)
            throw std::runtime_error("unable to connect to server");
    }

    virtual void send(const std::string &message)
    {
        size_t length = 0;
        size_t totalLength = 0;
        while (totalLength != message.size())
        {
            int code = length = ::send(socket, message.c_str() + totalLength, message.size() - totalLength, 0);
            if (code == SOCKET_ERROR)
            {
                code = lastError;
                throw std::runtime_error("send failed with error: " + std::to_string(code));
            }
            else
                totalLength += length;
        }
    }

    virtual std::string receive()
    {
        std::string message(bufferSize, 0);
        int code = recv(socket, message.data(), message.size(), 0);
        if (code >= 0)
        {
            message.resize(code);
        }
        else
        {
            code = lastError;
            throw std::runtime_error("recv failed with error: " + std::to_string(code));
        }
        return message;
    }

    virtual void receive(std::string &message)
    {
        message.resize(bufferSize, 0);
        int code = recv(socket, message.data(), message.size(), 0);
        if (code >= 0)
        {
            message.resize(code);
        }
        else
        {
            code = lastError;
            throw std::runtime_error("recv failed with error: " + std::to_string(code));
        }
    }

    virtual void close() override
    {
        int code = shutdown(socket, SD_SEND);
        if (code == SOCKET_ERROR)
            closesocket(socket);
        else
        {
            std::string temp(bufferSize, 0);
            do
            {
                code = recv(socket, temp.data(), temp.size(), 0);
            } while (code > 0);
        }
        socket = INVALID_SOCKET;
    }

protected:
    tcpClient(SOCKET socket) : tcpSocket(socket) {}
    friend class tcpServer;
};

/// SSL client
class sslClient : public sslSocket, public tcpClient
{
public:
    sslClient() : tcpClient() {}

    void connect(const std::string &adress, unsigned short port) override
    {
        tcpClient::connect(adress, port);
        sslContext = std::shared_ptr<SSL_CTX>(SSL_CTX_new(TLS_client_method()), freeSslContext);
        if (sslContext == nullptr)
            throw std::runtime_error("SSL_CTX_new failed with error: " + getSslError());
        ssl = std::shared_ptr<SSL>(SSL_new(sslContext.get()), freeSsl);
        if (ssl == nullptr)
            throw std::runtime_error("SSL_new failed with error: " + getSslError());
        int code = SSL_set_fd(ssl.get(), socket);
        if (code <= 0)
            throw std::runtime_error("SSL_set_fd failed with error: " + std::to_string(code) + " " + getSslError());
        code = SSL_connect(ssl.get());
        if (code <= 0)
            throw std::runtime_error("SSL_connect failed with error: " + std::to_string(code) + " " + getSslError());
    }

    void send(const std::string &message) override
    {
        if (sslContext == nullptr || ssl == nullptr)
            throw std::runtime_error("SSL socket is not initialized");

        int code = SSL_write(ssl.get(), message.c_str(), message.size());
        if (code <= 0)
            throw std::runtime_error("SSL_write failed with error: " + std::to_string(code) + " " + getSslError());
    }

    std::string receive() override
    {
        if (sslContext == nullptr || ssl == nullptr)
            throw std::runtime_error("SSL socket is not initialized");

        std::string message(bufferSize, 0);
        int code = SSL_read(ssl.get(), message.data(), message.size());
        if (code >= 0)
        {
            message.resize(code);
        }
        else
            throw std::runtime_error("SSL_read failed with error: " + std::to_string(code));
        return message;
    }

    void receive(std::string &message) override
    {
        if (sslContext == nullptr || ssl == nullptr)
            throw std::runtime_error("SSL socket is not initialized");

        message.resize(bufferSize, 0);
        int code = SSL_read(ssl.get(), message.data(), message.size());
        if (code >= 0)
        {
            message.resize(code);
        }
        else
            throw std::runtime_error("SSL_read failed with error: " + std::to_string(code));
    }

    void close() override
    {
        sslSocket::close();
        tcpClient::close();
    }

private:
    friend class sslServer;

    sslClient(SOCKET socket, std::shared_ptr<SSL_CTX> newSslContext, std::shared_ptr<SSL> newSsl)
        : tcpClient(socket), sslSocket(newSslContext, newSsl) {}
};

// TCP server socket
class tcpServer : public tcpSocket
{
public:
    tcpServer() : tcpSocket() {}

    void bind(unsigned short port)
    {
        struct addrinfo *result = nullptr, hints;

        std::memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        int code = getaddrinfo(nullptr, std::to_string(port).c_str(), &hints, &result);
        if (code != 0)
            throw std::runtime_error("getaddrinfo failed with error: " + std::to_string(code));

        socket = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (socket == INVALID_SOCKET)
        {
            code = lastError;
            freeaddrinfo(result);
            throw std::runtime_error("socket failed with error: " + std::to_string(code));
        }

        code = ::bind(socket, result->ai_addr, (int)result->ai_addrlen);
        if (code == SOCKET_ERROR)
        {
            code = lastError;
            freeaddrinfo(result);
            closesocket(socket);
            throw std::runtime_error("bind failed with error: " + std::to_string(code));
        }

        freeaddrinfo(result);

        code = ::listen(socket, SOMAXCONN);
        if (code == SOCKET_ERROR)
        {
            code = lastError;
            closesocket(socket);
            throw std::runtime_error("listen failed with error: " + std::to_string(code));
        }
    }

    virtual std::shared_ptr<tcpClient> listen()
    {
        return std::shared_ptr<tcpClient>(new tcpClient(accept()));
    }

protected:
    SOCKET accept()
    {
        SOCKET clientSocket = ::accept(socket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET)
        {
            int code = lastError;
            close();
            throw std::runtime_error("accept failed with error: " + std::to_string(code));
        }
        return clientSocket;
    }
};

/// SSL server
class sslServer : public sslSocket, public tcpServer
{
public:
    sslServer(const std::string &cert, const std::string &key, const std::vector<std::filesystem::path> &importPaths) : tcpServer()
    {
        std::filesystem::path certPath = cert, keyPath = key;
        for (auto it = importPaths.crbegin(); !(it == importPaths.crend()); it++)
        {
            if (std::filesystem::exists(*it / cert))
            {
                certPath = (*it / cert);
                break;
            }
        }
        for (auto it = importPaths.crbegin(); !(it == importPaths.crend()); it++)
        {
            if (std::filesystem::exists(*it / key))
            {
                keyPath = (*it / key);
                break;
            }
        }

        sslContext = std::shared_ptr<SSL_CTX>(SSL_CTX_new(TLS_server_method()), freeSslContext);
        if (sslContext == nullptr)
            throw std::runtime_error("SSL_CTX_new failed with error: " + getSslError());

        int code = SSL_CTX_use_certificate_file(sslContext.get(), certPath.string().c_str(), SSL_FILETYPE_PEM);
        if (code <= 0)
            throw std::runtime_error("SSL_CTX_use_certificate_file failed with error: " + std::to_string(code) + " " + getSslError());
        code = SSL_CTX_use_PrivateKey_file(sslContext.get(), keyPath.string().c_str(), SSL_FILETYPE_PEM);
        if (code <= 0)
            throw std::runtime_error("SSL_CTX_use_PrivateKey_file failed with error: " + std::to_string(code) + " " + getSslError());
    }

    std::shared_ptr<tcpClient> listen() override
    {
        if (sslContext == nullptr)
            throw std::runtime_error("SSL socket is not initialized");

        SOCKET clientSocket = accept();

        std::shared_ptr<SSL> ssl = std::shared_ptr<SSL>(SSL_new(sslContext.get()), freeSsl);
        if (ssl == nullptr)
            throw std::runtime_error("SSL_new failed with error: " + getSslError());
        int code = SSL_set_fd(ssl.get(), clientSocket);
        if (code <= 0)
            throw std::runtime_error("SSL_set_fd failed with error: " + std::to_string(code) + " " + getSslError());
        code = SSL_accept(ssl.get());
        if (code <= 0)
            throw std::runtime_error("SSL_accept failed with error: " + std::to_string(code) + " " + getSslError());

        return std::shared_ptr<tcpClient>(new sslClient(clientSocket, sslContext, ssl));
    }

    void close() override
    {
        sslSocket::close();
        tcpServer::close();
    }
};

#endif /* !TCP_H_ */
