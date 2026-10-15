#ifndef LISTENINGSOCKET_HPP
#define LISTENINGSOCKET_HPP

#include "bindingsocket.hpp"

namespace net {
class ListeningSocket : public BindingSocket {
private:
    int backlog_;

public:
    ListeningSocket(int domain, int service, int protocol, int port,
                    unsigned long interface_addr, int backlog);
    void start_listening();
    SOCKET accept_client() const;
};
} // namespace net

#endif