#include <arpa/inet.h>
#include <chrono>
#include <cstring>
#include <thread>
#include <unistd.h>
#include "client.hpp"

static constexpr int max_attempts{5};

void Client::CreateSocket()
{
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ < 0)
    {
        std::cerr << "Error: Socket not created\n";
    }

    server_address_.sin_family = AF_INET;
    server_address_.sin_port = htons(port);
}

void Client::ConnectToServer(const std::string& message)
{
    CreateSocket();
    if (inet_pton(AF_INET, "127.0.0.1", &server_address_.sin_addr) <= 0)
    {
        std::cerr << "Error: Invalid Server address\n";
    }

    if (connect(socket_, (struct sockaddr *)&server_address_, sizeof(server_address_)) < 0)
    {
        std::cerr << "Error: Connection Failed\n";
        close(socket_);
        return;
    }

    SendMessage(message);
    // GetResponse();
    close(socket_);
}

void Client::SendMessage(const std::string& message)
{
    send(socket_, message.c_str(), message.size(), 0);
}

// void Client::GetResponse()
// {
//     const int size = 1024;
//     char buffer[size] = {0};
//     int bytes_read = read(socket_, buffer, size - 1);
//     if (bytes_read < 0)
//     {
//         std::cerr << "Error: Reading from Server\n";
//     }
//     else
//     {
//         buffer[bytes_read] = '\0';
//         std::cout << buffer << std::endl;
//     }
// }
