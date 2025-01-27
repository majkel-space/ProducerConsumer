#ifndef DAN
#define DAN

#include <atomic>
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
    void LaunchMonitorDeliveryThread(std::future<Order>, Order&);

  private:
    void MonitorDelivery(std::future<Order>, Order&);
    void RegisterNewOrder(std::string&&);

    std::atomic_bool& stop_flag_;
    Server server_;
    Warehouse& warehouse_;
    std::thread collect_orders_thread_{};
};

#endif //DAN
