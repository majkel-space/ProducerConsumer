#ifndef WAREHOUSE
#define WAREHOUSE

#include <condition_variable>
#include <future>
#include <optional>
#include "dan.hpp"
#include "delivery_car.hpp"
#include "queue"

static constexpr std::uint8_t number_of_delivery_cars{3U};

class Warehouse
{
  public:
    Warehouse();
    ~Warehouse();

    void OpenWarehouse();
    void AddNewOrder(std::string&&);
    std::optional<std::string> GetNextOrder();

  private:
    std::condition_variable cv_;
    std::mutex mutex_;
    Queue<std::string> order_queue_{};
    Queue<std::future<std::string>> delivery_queue_{};
    std::array<DeliveryCar, number_of_delivery_cars> delivery_cars_{DeliveryCar{1U}, DeliveryCar{2U}, DeliveryCar{3U}};
};

#endif //WAREHOUSE
