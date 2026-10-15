#include "server.hpp"

namespace Server {
Server::Server(int domain, int service, int protocol, int port,
               unsigned long interface_addr, int backlog)
    : socket_(std::make_unique<net::ListeningSocket>(
          domain, service, protocol, port, interface_addr, backlog)) {}

net::ListeningSocket* Server::get_socket() const {
    return socket_.get();
}
} // namespace Server