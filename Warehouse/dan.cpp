#include <atomic>
#include <chrono>
#include <random>
#include <thread>
#include "dan.hpp"

extern std::atomic<bool> stop_flag;

int SetNonBlocking(int& socket)
{
    int flags = fcntl(socket, F_GETFL, 0);

    return flags == -1
           ? -1
           : fcntl(socket, F_SETFL, flags | O_NONBLOCK);
}

Dan::Dan()
{
    CreateSocket();
}

Dan::~Dan()
{
    close(server_socket_);
}

void Dan::CreateSocket()
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
}

void Dan::RegisterSocketWithEpoll()
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

void Dan::Listen()
{
    listen(server_socket_, 5);
    RegisterSocketWithEpoll();

    std::unordered_map<int, std::string> client_msgs_buffer{};

    while (not stop_flag)
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
                client_msgs_buffer[client_fd] = "";
            }
            else
            {
                HandleConnection(it, client_msgs_buffer);
            }
        }
    }
}

bool Dan::RegisterClientWithEpoll(int& client_fd)
{
    epoll_fd_.events = EPOLLIN | EPOLLET;;
    epoll_fd_.data.fd = client_fd;
    if (epoll_ctl(epoll_socket_, EPOLL_CTL_ADD, client_fd, &epoll_fd_) == -1)
    {
        return false;
    }
    return true;
}

void Dan::HandleConnection(int& event_no, std::unordered_map<int, std::string>& client_msgs_buffer)
{
    char buffer[BUFFER_SIZE];
    int client_fd = waiting_events_[event_no].data.fd;
    ssize_t count = recv(client_fd, buffer, BUFFER_SIZE, 0);
    if (count == -1)
    {
        std::cerr << "Error: recievev\n";
        close(client_fd);
        epoll_ctl(epoll_socket_, EPOLL_CTL_DEL, client_fd, NULL);
        client_msgs_buffer.erase(client_fd);
    }
    else if (count == 0)
    {
        std::cout << "Connection closed by client\n";
        close(client_fd);
        epoll_ctl(epoll_socket_, EPOLL_CTL_DEL, client_fd, NULL);
        client_msgs_buffer.erase(client_fd);
    }
    else
    {
        client_msgs_buffer[client_fd].append(buffer, count);
        std::cout << "Received from client: " << client_msgs_buffer.at(client_fd) << std::endl;
        SendConfirmation(client_fd, client_msgs_buffer.at(client_fd));
        client_msgs_buffer[client_fd].clear();
    }
}

void Dan::SendConfirmation(int& socket, const std::string& message)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(50, 500);
    std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
    send(socket, message.c_str(), message.size(), 0);
}
