
#include <functional>
#include "dan.hpp"

Dan::Dan(std::atomic_bool& stop_flag, Warehouse& warehouse)
    : stop_flag_{stop_flag}, warehouse_{warehouse}, server_{},
      collect_orders_thread_{std::thread(&Dan::ProcessOrders, this)}
{}

Dan::~Dan()
{
    stop_flag_.store(true);
    if (collect_orders_thread_.joinable())
    {
        collect_orders_thread_.join();
    }
}

void Dan::ProcessOrders()
{
    while (not stop_flag_.load())
    {
        server_.Listen(*this);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Dan::RegisterNewOrder(std::string&& msg)
{
    warehouse_.AddNewOrder(std::move(msg));
}

void Dan::MonitorDeliveries(std::future<Order> future)
{
    auto status = std::future_status::timeout;
    while (status != std::future_status::ready and not stop_flag_.load())
    {
        status = future.wait_for(std::chrono::milliseconds(100));
    }

    if (status == std::future_status::ready)
    {
        Order order = future.get();
        if (order.actual_delivered_time > order.expected_delivery_time * 1.2)
        {
            std::cerr << "Dan: Late Delivery! Order: " << order.msg << std::endl;
        }
        else
        {
            std::cout << "Dan: Order " << order.msg << " delivered on time!" << std::endl;
        }
    }
}
