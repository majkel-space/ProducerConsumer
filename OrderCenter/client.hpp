
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>

class Client
{
  public:
    Client();
    ~Client();

    void PlaceOrder(const std::string&);

  private:
    void CreateSocket();
    void ConnectToServer();
    void SendMessage(const std::string&);
    void GetConfirmation();

  int socket_ = 0;
  struct sockaddr_in server_address_;
  const u_int16_t port = 1234;
};
