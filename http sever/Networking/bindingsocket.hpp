#ifndef BINDINGSOCKET_HPP
#define BINDINGSOCKET_HPP

#include "socket.hpp"

namespace net {
class BindingSocket : public SocketServer {
public:
    BindingSocket(int domain, int service, int protocol, int port,
                  unsigned long interface_addr);
    int connect_to_network(SOCKET sock, const sockaddr_in& addr) override;
};
} // namespace net

#endif