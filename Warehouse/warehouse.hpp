#ifndef WAREHOUSE
#define WAREHOUSE

#include <condition_variable>
#include <future>
#include <optional>
#include "dan.hpp"
#include "delivery_car.hpp"
#include "queue"

static constexpr std::uint8_t number_of_delivery_cars{3U};

struct Order
{
    std::string msg{};
    int expected_delivery_time{};
    int actual_delivered_time{};
};


class Warehouse
{
  public:
    Warehouse();
    ~Warehouse();

    void OpenWarehouse();
    void AddNewOrder(std::string&&);

  private:
    std::optional<Order> GetNextOrder();
    void StartDelivering(std::vector<std::thread>&);

    std::condition_variable cv_;
    std::mutex mutex_;
    Queue<Order> order_queue_{};
    Queue<std::promise<Order>> delivery_queue_{};
    std::array<DeliveryCar, number_of_delivery_cars> delivery_cars_{DeliveryCar{1U}, DeliveryCar{2U}, DeliveryCar{3U}};
};

#endif //WAREHOUSE
