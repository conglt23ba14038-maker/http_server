#include "testserver.hpp"

#include <exception>
#include <iostream>

int main() {
    try {
        TestServer server;
        server.launch();
    } catch (const std::exception& error) {
        std::cerr << "Server error: " << error.what() << std::endl;
        return 1;
    }
}