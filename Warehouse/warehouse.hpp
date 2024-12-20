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

    std::array<DeliveryCar, 3> delivery_cars_{DeliveryCar{1U}, DeliveryCar{2U}, DeliveryCar{3U}};
    std::thread dan_thread_;
    std::vector<std::thread> car_threads_;
    Queue<std::string> order_queue_{};
    Queue<std::future<std::string>> delivery_queue_{};
};
