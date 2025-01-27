#include <atomic>
#include <optional>
#include <random>
#include <thread>
#include "warehouse.hpp"

extern std::atomic<bool> stop_flag;
static constexpr std::uint16_t min_delivery_time{100U};
static constexpr std::uint16_t max_delivery_time{500U};
static constexpr std::uint8_t no_of_delivery_cars{3U};

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
}

void Warehouse::OpenWarehouse()
{
    dan_ = std::make_unique<Dan>(stop_flag, *this);
    for (auto it = 0U; it < no_of_delivery_cars; ++it)
    {
        delivery_cars_.emplace_back(it + 1U, *this);
    }

    while (not stop_flag.load())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    cv_.notify_all();
}

void Warehouse::AddNewOrder(std::string&& msg)
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        const auto delivery_time = GenerateDeliveryTime();
        auto order = Order{std::move(msg), delivery_time, 0};
        order_queue_.Push(std::move(order));
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

std::promise<Order> Warehouse::GetOrderPromise(Order& order)
{
    std::lock_guard<std::mutex> lock(mutex_);
    std::promise<Order> promise;
    auto future = promise.get_future();
    NotifyDan(std::move(future), order);
    return promise;
}

void Warehouse::NotifyDan(std::future<Order>&& future, Order& order)
{
    dan_->LaunchMonitorDeliveryThread(std::move(future), order);
}
