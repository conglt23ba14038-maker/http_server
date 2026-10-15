#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <winsock2.h>
#include <ws2tcpip.h>

namespace net {
class SocketServer {
private:
    sockaddr_in address_{};
    SOCKET sock_ = INVALID_SOCKET;
    int connection_ = SOCKET_ERROR;

public:
    SocketServer(int domain, int service, int protocol, int port,
                 unsigned long interface_addr);
    virtual ~SocketServer();

    SocketServer(const SocketServer&) = delete;
    SocketServer& operator=(const SocketServer&) = delete;

    virtual int connect_to_network(SOCKET sock, const sockaddr_in& addr) = 0;
    void test_connection(int result) const;
    sockaddr_in get_address() const;
    SOCKET get_sock() const;
    int get_connection() const;
    void set_connection(int connection);
};
} // namespace net

#endif