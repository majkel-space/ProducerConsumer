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
    for (int it = 0; it < number_of_cachiers; ++it)
    {
        cachier_treads_.emplace_back(std::bind(&OrderCenter::CollectOrders, this, it + 1));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

OrderCenter::~OrderCenter()
{
    stop_flag.store(true);
    for (auto& thread : cachier_treads_)
    {
        if (thread.joinable())
            thread.join();
    }
}

void OrderCenter::CollectOrders(std::uint8_t cachier_id)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100, 1000);
    while (not stop_flag.load())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
        PlaceOrderInQueue(cachier_id);
    }
}

void OrderCenter::PlaceOrderInQueue(std::uint8_t cachier_id)
{
    queue_semaphore_.acquire();
    order_queue_.push("Cachier_" + std::to_string(cachier_id) + "_order_no_" + std::to_string(++number_of_order_));
    std::cout << "Order placed: " << order_queue_.back() << std::endl;
    queue_semaphore_.release();
}
