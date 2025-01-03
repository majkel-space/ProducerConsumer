
#include <functional>
#include "dan.hpp"
#include "warehouse.hpp"

Dan::Dan(std::atomic_bool& stop_flag, Warehouse& warehouse, Queue<std::future<std::string>>& delivery_queue)
    : stop_flag_{stop_flag}, warehouse_{warehouse}, delivery_queue_{delivery_queue}, server_{},
      collect_orders_thread_{std::thread(&Dan::ProcessOrders, this)},
      monitor_deliveries_thread_{std::thread(&Dan::MonitorDeliveries, this)}
{}

Dan::~Dan()
{
    stop_flag_.store(true);
    if (collect_orders_thread_.joinable())
    {
        collect_orders_thread_.join();
    }
    if (monitor_deliveries_thread_.joinable())
    {
        monitor_deliveries_thread_.join();
    }
}

void Dan::ProcessOrders()
{
    while (not stop_flag_.load())
    {
        server_.Listen(warehouse_);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Dan::MonitorDeliveries()
{
    while (not stop_flag_.load())
    {
        auto future = std::move(delivery_queue_.Pop());
        if (future)
        {
            auto future_value = std::move(future.value());
            try
            {
                std::string delivered_order = future_value.get();
                std::cout << "Dan: Order Delivered - " << delivered_order << std::endl;
            }
            catch (...)
            {
                std::cerr << "Dan: Delivery failed!" << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
