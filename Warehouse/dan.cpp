
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

void Dan::MonitorDelivery(std::future<Order> future, Order order)
{
    auto status = future.wait_for(std::chrono::milliseconds(static_cast<int>(order.expected_delivery_time * 1.2)));

    if (status == std::future_status::ready)
    {
        std::cout << "Dan: Order " << order.msg << " delivered on time!" << std::endl;
    }
    else
    {
        std::cerr << "Dan: Late Delivery! Order: " << order.msg << std::endl;
    }
}
