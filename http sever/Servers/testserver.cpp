#include "testserver.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace {
constexpr std::size_t max_request_size = 30000;

std::size_t content_length(const std::string& headers) {
    std::istringstream input(headers);
    std::string line;

    while (std::getline(input, line)) {
        const auto colon = line.find(':');
        if (colon == std::string::npos) continue;

        std::string name = line.substr(0, colon);
        std::transform(name.begin(), name.end(), name.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        if (name != "content-length") continue;

        const std::string value = line.substr(colon + 1);
        const auto first = value.find_first_not_of(" \t");
        if (first == std::string::npos)
            throw std::runtime_error("Invalid Content-Length");

        std::size_t used = 0;
        const auto length = std::stoull(value.substr(first), &used);
        if (length > max_request_size ||
            value.substr(first + used).find_first_not_of(" \t\r") != std::string::npos)
            throw std::runtime_error("Invalid Content-Length");

        return static_cast<std::size_t>(length);
    }
    return 0;
}
} // namespace

TestServer::TestServer()
    : Server::Server(AF_INET, SOCK_STREAM, IPPROTO_TCP,
                     8080, INADDR_LOOPBACK, 3) {}

void TestServer::reply(const std::string& status, const std::string& body) {
    response_status_ = status;
    response_body_ = body;
}

void TestServer::accepter() {
    client_ = get_socket()->accept_client();
    request_.clear();
    char chunk[4096];

    while (true) {
        const int n = ::recv(client_, chunk, sizeof(chunk), 0);
        if (n <= 0)
            throw std::runtime_error("recv failed or client disconnected");

        request_.append(chunk, n);
        if (request_.size() > max_request_size)
            throw std::runtime_error("Request is too large");

        const auto end = request_.find("\r\n\r\n");
        if (end == std::string::npos) continue;

        const auto header_size = end + 4;
        const auto body_size = content_length(request_.substr(0, end));
        if (body_size > max_request_size - header_size)
            throw std::runtime_error("Request is too large");

        if (request_.size() >= header_size + body_size) {
            request_.resize(header_size + body_size);
            return;
        }
    }
}

void TestServer::handler() {
    std::cout << "Request received:\n" << request_ << std::endl;

    std::istringstream first_line(
        request_.substr(0, request_.find("\r\n")));
    std::string method, path, version;

    if (!(first_line >> method >> path >> version) ||
        version.rfind("HTTP/", 0) != 0)
        return reply("400 Bad Request", "Invalid HTTP request\n");

    path = path.substr(0, path.find('?'));
    const std::string body =
        request_.substr(request_.find("\r\n\r\n") + 4);

    if (path == "/items") {
        if (method == "GET") {
            std::string list;
            for (const auto& item : items_)
                list += item.first + ": " + item.second + "\n";
            return reply("200 OK", list);
        }
        if (method == "POST") {
            const std::string id = std::to_string(next_item_id_++);
            items_[id] = body;
            return reply("201 Created", id + "\n");
        }
        return reply("405 Method Not Allowed", "Method not allowed\n");
    }

    const std::string prefix = "/items/";
    if (path.rfind(prefix, 0) != 0 ||
        path.size() == prefix.size() ||
        path.find('/', prefix.size()) != std::string::npos)
        return reply("404 Not Found", "Route not found\n");

    const std::string id = path.substr(prefix.size());
    if (method != "GET" && method != "PUT" && method != "DELETE")
        return reply("405 Method Not Allowed", "Method not allowed\n");

    const auto item = items_.find(id);
    if (item == items_.end())
        return reply("404 Not Found", "Item not found\n");

    if (method == "GET")
        return reply("200 OK", item->second);

    if (method == "PUT") {
        item->second = body;
        return reply("200 OK", "Updated\n");
    }

    items_.erase(item);
    reply("204 No Content", "");
}

void TestServer::responder() {
    std::string response =
        "HTTP/1.1 " + response_status_ + "\r\n"
        "Connection: close\r\n";

    if (response_status_ != "204 No Content") {
        response += "Content-Type: text/plain; charset=utf-8\r\n";
        response += "Content-Length: " +
                    std::to_string(response_body_.size()) + "\r\n";
    }
    response += "\r\n" + response_body_;

    std::size_t sent_total = 0;
    while (sent_total < response.size()) {
        const int sent = ::send(
            client_,
            response.data() + sent_total,
            static_cast<int>(response.size() - sent_total),
            0
        );
        if (sent <= 0)
            throw std::runtime_error(
                "send failed: " + std::to_string(WSAGetLastError()));
        sent_total += static_cast<std::size_t>(sent);
    }

    ::shutdown(client_, SD_SEND);
    ::closesocket(client_);
    client_ = INVALID_SOCKET;
}

void TestServer::launch() {
    get_socket()->start_listening();
    std::cout << "Server running at http://127.0.0.1:8080/\n";

    while (true) {
        try {
            std::cout << "Waiting for a connection...\n";
            accepter();
            handler();
            responder();
            std::cout << "Connection closed.\n";
        } catch (const std::exception& error) {
            if (client_ != INVALID_SOCKET) {
                ::closesocket(client_);
                client_ = INVALID_SOCKET;
            }
            std::cerr << "Client error: " << error.what() << '\n';
        }
    }
}