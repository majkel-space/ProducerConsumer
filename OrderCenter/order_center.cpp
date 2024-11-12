#include <atomic>
#include <functional>
#include <iostream>
#include <numeric>
#include <random>
#include "order_center.hpp"

extern std::atomic<bool> stop_flag;
static const std::uint8_t number_of_cachiers{5};

OrderCenter::OrderCenter()
{
    stop_flag.store(false);
    ReleaseQueues();

    for (int it = 0; it < number_of_cachiers; ++it)
    {
        cachier_treads_.emplace_back(std::bind(&OrderCenter::CollectOrders, this, it + 1));
    }
    for (int it = 0; it < number_of_simons; ++it)
    {
        simons_treads_.emplace_back(std::bind(&OrderCenter::ProcessOrders, this, it + 1));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

OrderCenter::~OrderCenter()
{
    stop_flag.store(true);
    for (auto& thread : cachier_treads_)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
    for (auto& thread : simons_treads_)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}

void OrderCenter::CollectOrders(std::uint8_t cachier_id)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100, 2000);
    while (not stop_flag.load())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
        PushOrderToQueue(cachier_id);
    }
}

void OrderCenter::PushOrderToQueue(std::uint8_t cachier_id)
{
    if (not queue_empty_slots_.acquire(stop_flag))
        return;

    std::unique_lock<std::mutex> lock(queue_mutex_);
    order_queue_.push("Cachier_" + std::to_string(cachier_id) + "_order_no_" + std::to_string(++number_of_order_));
    std::cout << "Order placed: " << order_queue_.back() << " Q_Size " << order_queue_.size() << std::endl;
    lock.unlock();
    queue_filled_slots_.release();
}

void OrderCenter::ProcessOrders(const std::uint8_t simon_id)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(50, 500);
    while (not stop_flag.load())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
        PopOrderFromQueue(simon_id);
    }
}

void OrderCenter::PopOrderFromQueue(const std::uint8_t simon_id)
{
    if (not queue_filled_slots_.acquire(stop_flag))
        return;
    std::unique_lock<std::mutex> lock(queue_mutex_);
    if (not order_queue_.empty())
    {
        std::cout << "Order Handled: " << order_queue_.front();
        order_queue_.pop();
        std::cout << " Q_size " << order_queue_.size() << std::endl;
    }
    queue_empty_slots_.release();
}

void OrderCenter::ReleaseQueues()
{
    for (int i = 0; i < number_of_simons; ++i)
    {
        queue_filled_slots_.release();
    }
    for (int i = 0; i < number_of_cachiers; ++i)
    {
        queue_empty_slots_.release();
    }
}
