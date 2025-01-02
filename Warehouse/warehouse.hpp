#include <future>
#include "dan.hpp"
#include "delivery_car.hpp"
#include "queue"

class Warehouse
{
  public:
    Warehouse() = default;

    void OpenWarehouse();

  private:
    Queue<std::string> order_queue_{};
    Queue<std::future<std::string>> delivery_queue_{};
};
