#ifndef DAN
#define DAN

#include <atomic>
#include <iostream>
#include <future>
#include "queue.hpp"
#include "server.hpp"
#include "warehouse.hpp"

struct Order;
class Warehouse;

class Dan
{
  public:
    Dan(std::atomic_bool&, Warehouse&);
    ~Dan();

    void ProcessOrders();
    void RegisterNewOrder(std::string&&);
    void MonitorDelivery(std::future<Order>);

  private:
    std::atomic_bool& stop_flag_;
    Server server_;
    Warehouse& warehouse_;
    std::thread collect_orders_thread_{};
};

#endif //DAN
