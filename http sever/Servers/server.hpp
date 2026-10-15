#ifndef SERVER_HPP
#define SERVER_HPP

#include "../Networking/listeningsocket.hpp"
#include <memory>

namespace Server {
class Server {
private:
    std::unique_ptr<net::ListeningSocket> socket_;

protected:
    virtual void accepter() = 0;
    virtual void handler() = 0;
    virtual void responder() = 0;

public:
    Server(int domain, int service, int protocol, int port,
           unsigned long interface_addr, int backlog);
    virtual ~Server() = default;

    virtual void launch() = 0;
    net::ListeningSocket* get_socket() const;
};
} // namespace Server

#endif