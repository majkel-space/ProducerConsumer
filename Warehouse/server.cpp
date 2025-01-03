#include <atomic>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include "dan.hpp"
#include "server.hpp"

int SetNonBlocking(int& socket)
{
    int flags = fcntl(socket, F_GETFL, 0);

    return flags == -1
           ? -1
           : fcntl(socket, F_SETFL, flags | O_NONBLOCK);
}

Server::Server()
{
    CreateSocket();
}

Server::~Server()
{
    close(server_socket_);
    std::cout << "Server DTOR\n";
}

void Server::CreateSocket()
{
    if ((server_socket_ = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cerr << "Error: Socket not created\n";
        exit(EXIT_FAILURE);
    }
    puts("Socket created");
    SetNonBlocking(server_socket_);

    int temp{1};
    if (setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, &temp, sizeof(temp)) < 0)
    {
        std::cerr << "Error: setsockopt failed\n";
        close(server_socket_);
        return;
    }

    server_.sin_family = AF_INET;
    server_.sin_addr.s_addr = INADDR_ANY;
    server_.sin_port = htons(port_);

    if (bind(server_socket_, (struct sockaddr*)&server_, sizeof(server_)) < 0)
    {
        std::cerr << "Error: Bind Failed\n";
        close(server_socket_);
        return;
    }
    puts("Bind done");

    listen(server_socket_, 5);
    RegisterSocketWithEpoll();
}

void Server::RegisterSocketWithEpoll()
{
    epoll_socket_ = epoll_create1(0);
    if (epoll_socket_ == -1)
    {
        std::cerr << "Error: epoll_create1\n";
        exit(EXIT_FAILURE);
    }
    epoll_fd_.events = EPOLLIN;
    epoll_fd_.data.fd = server_socket_;
    if (epoll_ctl(epoll_socket_, EPOLL_CTL_ADD, server_socket_, &epoll_fd_) == -1)
    {
        std::cerr << "Error: epoll_ctl server_socket_\n";
        exit(EXIT_FAILURE);
    }
    puts("Socket registered with epoll");
}

void Server::Listen(Dan& dan)
{
    int number_of_events = epoll_wait(epoll_socket_, waiting_events_, max_orders, -1);
    for (int it = 0; it < number_of_events; ++it) {
        if (waiting_events_[it].data.fd == server_socket_)
        {
            int client_fd = accept(server_socket_, NULL, NULL);
            if (client_fd == -1)
            {
                std::cerr << "Error: Accept\n";
                continue;
            }
            SetNonBlocking(client_fd);
            if (not RegisterClientWithEpoll(client_fd))
            {
                std::cerr << "Error: Epoll file descriptor\n";
                close(client_fd);
            }
        }
        else
        {
            HandleConnection(dan, it);
        }
    }
}

bool Server::RegisterClientWithEpoll(int& client_fd)
{
    epoll_fd_.events = EPOLLIN | EPOLLET;;
    epoll_fd_.data.fd = client_fd;
    if (epoll_ctl(epoll_socket_, EPOLL_CTL_ADD, client_fd, &epoll_fd_) == -1)
    {
        return false;
    }
    return true;
}

void Server::HandleConnection(Dan& dan, int& event_no)
{
    char buffer[BUFFER_SIZE];
    int client_fd = waiting_events_[event_no].data.fd;
    ssize_t count = recv(client_fd, buffer, BUFFER_SIZE, 0);
    if (count == -1)
    {
        std::cerr << "Error: recieved\n";
        close(client_fd);
        epoll_ctl(epoll_socket_, EPOLL_CTL_DEL, client_fd, NULL);
    }
    else
    {
        std::string message(buffer, count);
        std::cout << "Received from client: " << message << std::endl;
        SendConfirmation(client_fd, message);
        dan.RegisterNewOrder(std::move(message));
    }
}

void Server::SendConfirmation(int& socket, const std::string& message)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(50, 500);
    std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
    send(socket, message.c_str(), message.size(), 0);
}
