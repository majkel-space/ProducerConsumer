#ifndef SERVER
#define SERVER

#include <cstdint>
#include <cstring>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include "queue.hpp"

#define BUFFER_SIZE 1024
static constexpr std::uint8_t max_orders{10};

class Warehouse;

class Server
{
  public:
    Server();
    ~Server();

    void Listen(Warehouse&);

  private:
    void CreateSocket();
    void RegisterSocketWithEpoll();
    bool RegisterClientWithEpoll(int&);
    void HandleConnection(Warehouse&, int&);
    void SendConfirmation(int&, const std::string&);

    struct sockaddr_in server_;
    int server_socket_, epoll_socket_;
    const uint16_t port_ = 1234;
    struct epoll_event epoll_fd_, waiting_events_[max_orders];
};

#endif //SERVER
