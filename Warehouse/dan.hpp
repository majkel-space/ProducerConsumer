#include <iostream>
#include <cstring>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <unordered_map>

#define BUFFER_SIZE 1024
static constexpr uint8_t number_of_simons{2};

class Dan
{
  public:
    Dan();
    ~Dan();

    void Listen();

  private:
    void CreateSocket();
    int SetNonBlocking(int& socket);
    void InitializeAndRegisterEpoll();
    bool RegisterClientWithEpoll(int&);
    void HandleConnection(int&, std::unordered_map<int, std::string>&);

    struct sockaddr_in server_;
    int socket_, epoll_;
    const uint16_t port_ = 1234;
    struct epoll_event epoll_fd_, waiting_events_[number_of_simons];
};
