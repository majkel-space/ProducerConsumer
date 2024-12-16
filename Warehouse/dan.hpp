#include <iostream>
#include <unordered_map>
#include "server.hpp"

class Dan
{
  public:
    Dan() = default;
    ~Dan() = default;

    void ProcessOrders();

  private:
    std::unordered_map<int, std::string> client_orders_{};
    Server server_{};
};
