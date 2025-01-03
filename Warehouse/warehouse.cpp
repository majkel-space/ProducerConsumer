#include <atomic>
#include <random>
#include <thread>
#include "warehouse.hpp"

extern std::atomic<bool> stop_flag;

Warehouse::Warehouse()
{
    stop_flag.store(false);
}

Warehouse::~Warehouse()
{
    stop_flag.store(true);
    cv_.notify_all();
}

void Warehouse::OpenWarehouse()
{
    Dan dan{stop_flag, *this, delivery_queue_};
    for (auto& delivery_car : delivery_cars_)
    {
        delivery_car.StartDelivering(*this, delivery_queue_);
    }

    while (not stop_flag.load())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Warehouse::AddNewOrder(std::string&& order)
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        order_queue_.Push(std::move(order));
    }
    cv_.notify_one();
}

std::optional<std::string> Warehouse::GetNextOrder()
{
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this]() {
        return stop_flag.load() or not order_queue_.IsEmpty();
    });

    if (stop_flag.load())
    {
        return std::nullopt;
    }

    return order_queue_.Pop();
}
