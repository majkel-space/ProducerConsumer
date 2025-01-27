#ifndef WAREHOUSE
#define WAREHOUSE

#include <condition_variable>
#include <future>
#include <list>
#include <memory>
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
    std::optional<Order> GetNextOrder();
    bool HasNoOrders() { return order_queue_.IsEmpty(); }
    std::condition_variable& GetCV() { return cv_; }
    std::promise<Order> GetOrderPromise(Order&);

  private:
    void NotifyDan(std::future<Order>&&, Order&);

    std::list<DeliveryCar> delivery_cars_;
    std::unique_ptr<Dan> dan_;
    std::condition_variable cv_;
    Queue<Order> order_queue_{};
    std::mutex mutex_;
};

#endif  // WAREHOUSE
