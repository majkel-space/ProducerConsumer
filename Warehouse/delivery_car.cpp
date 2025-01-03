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

DeliveryCar::DeliveryCar(std::uint8_t id)
    : id_{id}
{
    stop_flag_.store(false);
}

DeliveryCar::~DeliveryCar()
{
    stop_flag_.store(true);
}

void DeliveryCar::DeliverOrder(Order& order, std::promise<Order>& promis)
{
    order.actual_delivered_time = GenerateDeliveryDelay(order.expected_delivery_time);
    std::this_thread::sleep_for(std::chrono::milliseconds(order.actual_delivered_time));

    promis.set_value(order);
    std::cout << "DeliveryCar " << static_cast<int>(id_) << ": " << order.msg << " - DELIVERED"
        << " Expected delivery time: " << order.expected_delivery_time << "ms. Actual time: " << order.actual_delivered_time  << "ms" << std::endl;
}

void DeliveryCar::Join()
{
    if (delivery_thread_.joinable())
        delivery_thread_.join();
}
