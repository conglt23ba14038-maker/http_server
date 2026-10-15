#include "socket.hpp"

#include <stdexcept>
#include <string>

namespace net {
SocketServer::SocketServer(int domain, int service, int protocol, int port,
                           unsigned long interface_addr) {
    WSADATA data;
    const int startup_result = WSAStartup(MAKEWORD(2, 2), &data);
    if (startup_result != 0) {
        throw std::runtime_error("WSAStartup failed: " +
                                 std::to_string(startup_result));
    }

    address_.sin_family = static_cast<ADDRESS_FAMILY>(domain);
    address_.sin_port = htons(static_cast<u_short>(port));
    address_.sin_addr.s_addr = htonl(interface_addr);

    sock_ = ::socket(domain, service, protocol);
    if (sock_ == INVALID_SOCKET) {
        const int error = WSAGetLastError();
        WSACleanup();
        throw std::runtime_error("socket failed: " + std::to_string(error));
    }
}

SocketServer::~SocketServer() {
    if (sock_ != INVALID_SOCKET) {
        closesocket(sock_);
    }
    WSACleanup();
}

void SocketServer::test_connection(int result) const {
    if (result == SOCKET_ERROR) {
        throw std::runtime_error("Winsock error: " +
                                 std::to_string(WSAGetLastError()));
    }
}

sockaddr_in SocketServer::get_address() const { return address_; }
SOCKET SocketServer::get_sock() const { return sock_; }
int SocketServer::get_connection() const { return connection_; }
void SocketServer::set_connection(int connection) {
    connection_ = connection;
}
} // namespace net