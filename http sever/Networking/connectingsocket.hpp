#ifndef CONNECTINGSOCKET_HPP
#define CONNECTINGSOCKET_HPP

#include "socket.hpp"

namespace net {
class ConnectingSocket : public SocketServer {
public:
    ConnectingSocket(int domain, int service, int protocol, int port,
                     unsigned long interface_addr);
    int connect_to_network(SOCKET sock, const sockaddr_in& addr) override;
};
} // namespace net

#endif