#include <atomic>
#include <unordered_map>
#include "dan.hpp"

extern std::atomic<bool> stop_flag;

Dan::Dan()
{
    CreateSocket();
}

Dan::~Dan()
{
    close(socket_);
}

void Dan::CreateSocket()
{
    if ((socket_ = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cerr <<"Socket creation failed/n";
        exit(EXIT_FAILURE);
    }
    SetNonBlocking(socket_);

    server_.sin_family = AF_INET;
    server_.sin_addr.s_addr = INADDR_ANY;
    server_.sin_port = htons(port_);

    if (bind(socket_, (struct sockaddr*)&server_, sizeof(server_)) < 0);
    {
        std::cerr << "Error: Bind Failed\n";
        close(socket_);
        return;
    }
}

int Dan::SetNonBlocking(int socket)
{
    int flags = fcntl(socket, F_GETFL, 0);

    return flags == -1
           ? -1
           : fcntl(socket, F_SETFL, flags | O_NONBLOCK);
}

void Dan::Listen()
{
    listen(socket_, 5);
    InitializeAndRegisterEpoll();

    char buffer[BUFFER_SIZE];
    std::unordered_map<int, std::string> client_msgs_buffer;

    while (not stop_flag and not client_msgs_buffer.empty())
    {
        int number_of_events = epoll_wait(epoll_, waiting_events_, number_of_simons, -1);
        for (int it = 0; it < number_of_events; ++it) {
            if (waiting_events_[it].data.fd == socket_)
            {
                int client_fd = accept(socket_, NULL, NULL);
                if (client_fd == -1)
                {
                    std::cerr << "Error: Accept\n";
                    continue;
                }
                SetNonBlocking(client_fd);

                // Register client socket with epoll
                epoll_fd_.events = EPOLLIN | EPOLLET;  // Edge-triggered mode for efficiency
                epoll_fd_.data.fd = client_fd;
                if (epoll_ctl(epoll_, EPOLL_CTL_ADD, client_fd, &epoll_fd_) == -1)
                {
                    std::cerr << "Error: Epoll file descriptor\n";
                    close(client_fd);
                    continue;
                }
                client_msgs_buffer[client_fd] = "";

            }
            else
            {
                int client_fd = waiting_events_[it].data.fd;
                ssize_t count = recv(client_fd, buffer, BUFFER_SIZE, 0);
                if (count == -1)
                {
                    std::cerr << "Error: recievev\n";
                    close(client_fd);
                    epoll_ctl(epoll_, EPOLL_CTL_DEL, client_fd, NULL);
                    client_msgs_buffer.erase(client_fd);
                }
                else if (count == 0)
                {
                    // Connection closed by client
                    close(client_fd);
                    epoll_ctl(epoll_, EPOLL_CTL_DEL, client_fd, NULL);
                    client_msgs_buffer.erase(client_fd);
                }
                else
                {
                    // Accumulate data in the client buffer
                    client_msgs_buffer[client_fd].append(buffer, count);
                    // Process complete messages (based on some delimiter or size)
                    // For simplicity, just print out the data received
                    std::cout << "Received from client " << client_fd << ": " << client_msgs_buffer[client_fd] << std::endl;
                    client_msgs_buffer[client_fd].clear();  // Clear buffer after processing
                }
            }
        }
    }
}

void Dan::InitializeAndRegisterEpoll()
{
    epoll_ = epoll_create1(0);
    if (epoll_ == -1)
    {
        std::cerr << "epoll_create1\n";
        exit(EXIT_FAILURE);
    }

    epoll_fd_.events = EPOLLIN;
    epoll_fd_.data.fd = socket_;
    if (epoll_ctl(epoll_, EPOLL_CTL_ADD, socket_, &epoll_fd_) == -1)
    {
        std::cerr << "epoll_ctl: socket_\n";
        exit(EXIT_FAILURE);
    }
}
