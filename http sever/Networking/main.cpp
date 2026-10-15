#include "listeningsocket.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

int main() {
    try {
        net::ListeningSocket server(AF_INET, SOCK_STREAM, IPPROTO_TCP,
                                    8080, INADDR_LOOPBACK, 10);
        server.start_listening();
        std::cout << "Listening at http://127.0.0.1:8080/" << std::endl;

        SOCKET client = server.accept_client();

        char request[4096];
        const int received = ::recv(client, request, sizeof(request), 0);
        if (received <= 0) {
            const int error = WSAGetLastError();
            closesocket(client);
            throw std::runtime_error("recv failed: " +
                                     std::to_string(error));
        }

        const std::string body = "Hello from Winsock!\n";
        const std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain; charset=utf-8\r\n"
            "Content-Length: " + std::to_string(body.size()) + "\r\n"
            "Connection: close\r\n\r\n" + body;

        std::size_t sent_total = 0;
        while (sent_total < response.size()) {
            const int sent = ::send(
                client,
                response.data() + sent_total,
                static_cast<int>(response.size() - sent_total),
                0
            );
            if (sent <= 0) {
                const int error = WSAGetLastError();
                closesocket(client);
                throw std::runtime_error("send failed: " +
                                         std::to_string(error));
            }
            sent_total += static_cast<std::size_t>(sent);
        }

        ::shutdown(client, SD_SEND);
        closesocket(client);
        std::cout << "Served one request; exiting." << std::endl;
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << std::endl;
        return 1;
    }
}