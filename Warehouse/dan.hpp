#include <iostream>
#include "queue.hpp"
#include "server.hpp"

class Dan
{
  public:
    Dan();
    ~Dan() = default;

    void ProcessOrders();

  private:
    Server server_;
    Queue<std::string> queue_{};
};
