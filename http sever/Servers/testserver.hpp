#ifndef TESTSERVER_HPP
#define TESTSERVER_HPP

#include "server.hpp"
#include <string>
#include <unordered_map>

class TestServer : public Server::Server {
private:
    SOCKET client_ = INVALID_SOCKET;
    std::string request_, response_status_, response_body_;
    std::unordered_map<std::string, std::string> items_;
    std::size_t next_item_id_ = 1;

    void reply(const std::string& status, const std::string& body);
    void accepter() override;
    void handler() override;
    void responder() override;

public:
    TestServer();
    void launch() override;
};

#endif