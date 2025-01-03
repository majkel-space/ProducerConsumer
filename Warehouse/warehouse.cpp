#include <atomic>
#include <optional>
#include <random>
#include <thread>
#include "warehouse.hpp"

extern std::atomic<bool> stop_flag;
static constexpr std::uint16_t min_delivery_time{100U};
static constexpr std::uint16_t max_delivery_time{500U};

int GenerateDeliveryTime()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(min_delivery_time, max_delivery_time);
    return distribution(gen);
}

Warehouse::Warehouse()
{
    stop_flag.store(false);
}

Warehouse::~Warehouse()
{
    stop_flag.store(true);
    cv_.notify_all();
    for (auto& delivery_car : delivery_cars_)
    {
        delivery_car.Join();
    }
}

void Warehouse::OpenWarehouse()
{
    Dan dan{stop_flag, *this};

    std::thread monitor_thread([this, &dan]() {
        while (!stop_flag.load())
        {
            std::optional<std::promise<Order>> promise;
            {
                promise = delivery_queue_.Pop();
            }
            if (promise)
            {
                std::thread([&dan, promise = std::move(promise.value())]() mutable {
                    auto future = promise.get_future();
                    dan.MonitorDeliveries(std::move(future));
                }).detach();
            }
        }
    });

    std::vector<std::thread> delivery_threads{};
    StartDelivering(delivery_threads);

    while (not stop_flag.load())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    for (auto& delivery_car : delivery_cars_)
    {
        delivery_car.Join();
    }
    if (monitor_thread.joinable())
    {
        monitor_thread.join();
    }
}

void Warehouse::AddNewOrder(std::string&& order)
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        const auto delivery_time = GenerateDeliveryTime();
        order_queue_.Push(Order{std::move(order), delivery_time, 0});
    }
    cv_.notify_one();
}

std::optional<Order> Warehouse::GetNextOrder()
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

void Warehouse::StartDelivering(std::vector<std::thread>& delivery_threads)
{
    for (auto& delivery_car : delivery_cars_)
    {
        delivery_threads.emplace_back([this, &delivery_car]() {
            while (!stop_flag.load())
            {
                auto order = GetNextOrder();
                if (not order)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    continue;
                }
                std::promise<Order> promise;
                delivery_car.DeliverOrder(order.value(), promise);
                delivery_queue_.Push(std::move(promise));
            }
        });
    }
}
