#include <atomic>
#include <iostream>
#include <future>
#include "queue.hpp"
#include "server.hpp"

class Dan
{
  public:
    Dan(std::atomic_bool&, Queue<std::string>&, Queue<std::future<std::string>>&);
    ~Dan();

    void ProcessOrders();
    void MonitorDeliveries();

  private:
    std::atomic_bool& stop_flag_;
    Server server_;
    Queue<std::string>& order_queue_;
    Queue<std::future<std::string>>& delivery_queue_;
    std::thread collect_orders_thread_{};
    std::thread monitor_deliveries_thread_{};
};
