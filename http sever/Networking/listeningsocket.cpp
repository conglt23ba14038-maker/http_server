#include "listeningsocket.hpp"

#include <stdexcept>
#include <string>

namespace net {
ListeningSocket::ListeningSocket(int domain, int service, int protocol,
                                 int port, unsigned long interface_addr,
                                 int backlog)
    : BindingSocket(domain, service, protocol, port, interface_addr),
      backlog_(backlog) {}

void ListeningSocket::start_listening() {
    test_connection(::listen(get_sock(), backlog_));
}

SOCKET ListeningSocket::accept_client() const {
    SOCKET client = ::accept(get_sock(), nullptr, nullptr);
    if (client == INVALID_SOCKET) {
        throw std::runtime_error("accept failed: " +
                                 std::to_string(WSAGetLastError()));
    }
    return client;
}
} // namespace net