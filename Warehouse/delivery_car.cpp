#include <chrono>
#include <iostream>
#include <functional>
#include <random>
#include <thread>
#include "delivery_car.hpp"
#include "warehouse.hpp"

static constexpr std::uint16_t min_delivery_multipl{0};
static constexpr std::uint16_t max_delivery_multipl{5};

int GenerateDeliveryDelay(int delivery_time)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(min_delivery_multipl, max_delivery_multipl);
    return delivery_time + delivery_time * distribution(gen) / 10;
}

DeliveryCar::DeliveryCar(std::uint8_t id, Warehouse& warehouse, std::atomic_bool& stop_flag)
    : id_{id}, warehouse_{warehouse}, stop_flag_{stop_flag}, cv_{warehouse.GetCV()},
      delivery_thread_{std::thread(&DeliveryCar::StartDelivering, this)}
{}

DeliveryCar::~DeliveryCar()
{
    stop_flag_.store(true);
    if (delivery_thread_.joinable())
        delivery_thread_.join();
}

void DeliveryCar::StartDelivering()
{
    while (not stop_flag_.load())
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this]() { return stop_flag_ or warehouse_.HasOrders(); });
        if (stop_flag_.load()) { break; }
        auto order = warehouse_.GetNextOrder();
        if (order)
        {
            lock.unlock();
            DeliverOrder(order.value());
        }
    }
}

void DeliveryCar::DeliverOrder(Order& order)
{
    order.actual_delivered_time = GenerateDeliveryDelay(order.expected_delivery_time);
    std::this_thread::sleep_for(std::chrono::milliseconds(order.actual_delivered_time));

    std::cout << "DeliveryCar " << static_cast<int>(id_) << ": " << order.msg << " - DELIVERED"
        << " Expected delivery time: " << order.expected_delivery_time << "ms. Actual time: " << order.actual_delivered_time  << "ms" << std::endl;
}
