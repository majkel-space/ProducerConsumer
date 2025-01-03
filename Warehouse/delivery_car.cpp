#include <chrono>
#include <iostream>
#include <functional>
#include <random>
#include <thread>
#include "delivery_car.hpp"
#include "warehouse.hpp"


static constexpr std::uint16_t min_delivery_time{100U};
static constexpr std::uint16_t max_delivery_time{500U};
static constexpr std::uint16_t min_delivery_multipl{0};
static constexpr std::uint16_t max_delivery_multipl{5};

int GenerateDeliveryDelay(int delivery_time)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(min_delivery_multipl, max_delivery_multipl);
    return delivery_time + delivery_time * distribution(gen) / 10;
}

int GenerateDeliveryTime()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(min_delivery_time, max_delivery_time);
    return distribution(gen);
}

DeliveryCar::DeliveryCar(std::uint8_t id)
    : id_{id}
{
    stop_flag_.store(false);
}

DeliveryCar::~DeliveryCar()
{
    stop_flag_.store(true);
}

void DeliveryCar::StartDelivering(Warehouse& warehouse, Queue<std::future<std::string>>& delivery_queue)
{
    delivery_thread_ = std::thread([this, &warehouse, &delivery_queue]() {
        while (true)
        {
            auto order = warehouse.GetNextOrder();
            if (not order) { break; }
            DeliverOrder(order.value(), delivery_queue);

        }
    });
}

void DeliveryCar::DeliverOrder(const std::string& order, Queue<std::future<std::string>>& delivery_queue)
{
    int delivery_time = GenerateDeliveryTime();
    int actual_time = GenerateDeliveryDelay(delivery_time);
    std::this_thread::sleep_for(std::chrono::milliseconds(actual_time));
    if (actual_time > delivery_time * 1.2)
    {
        std::cerr << "DeliveryCar " << static_cast<int>(id_) << ": Late Delivery! Order: " << order << std::endl;
    }
    std::promise<std::string> promise;
    std::future<std::string> future = promise.get_future();
    promise.set_value(order);
    delivery_queue.Push(std::move(future));

    std::cout << "DeliveryCar " << static_cast<int>(id_) << ": " << order << " - DELIVERED"
        << " Delivery time: " << delivery_time << "ms. Actual time: " << actual_time  << "ms" << std::endl;
}

void DeliveryCar::Join()
{
    if (delivery_thread_.joinable())
        delivery_thread_.join();
}
