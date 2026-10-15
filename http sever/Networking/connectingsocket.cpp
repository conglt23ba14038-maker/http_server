#include "connectingsocket.hpp"

namespace net {
ConnectingSocket::ConnectingSocket(int domain, int service, int protocol,
                                   int port, unsigned long interface_addr)
    : SocketServer(domain, service, protocol, port, interface_addr) {
    set_connection(connect_to_network(get_sock(), get_address()));
    test_connection(get_connection());
}

int ConnectingSocket::connect_to_network(SOCKET sock,
                                         const sockaddr_in& addr) {
    return ::connect(sock, reinterpret_cast<const sockaddr*>(&addr),
                     sizeof(addr));
}
} // namespace net