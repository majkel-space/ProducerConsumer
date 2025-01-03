#ifndef DAN
#define DAN

#include <atomic>
#include <iostream>
#include <future>
#include "queue.hpp"
#include "server.hpp"

class Warehouse;

class Dan
{
  public:
    Dan(std::atomic_bool&, Warehouse&, Queue<std::future<std::string>>&);
    ~Dan();

    void ProcessOrders();
    void MonitorDeliveries();

  private:
    std::atomic_bool& stop_flag_;
    Server server_;
    Warehouse& warehouse_;
    Queue<std::future<std::string>>& delivery_queue_;
    std::thread collect_orders_thread_{};
    std::thread monitor_deliveries_thread_{};
};

#endif //DAN
