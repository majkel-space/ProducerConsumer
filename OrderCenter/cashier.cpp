#include "cashier.hpp"

#include <chrono>
#include <functional>
#include <iostream>
#include <optional>
#include <random>

Cashier::Cashier(const std::uint8_t cashier_id, std::atomic_bool& stop_flag, OrderCenterManager<Simon>& manager)
    : cashier_id_{cashier_id},
      stop_flag_{stop_flag},
      manager_{manager},
      cashier_tread_{std::bind(&Cashier::ProduceOrder, this)}
{}

Cashier::~Cashier()
{
    if (cashier_tread_.joinable())
        cashier_tread_.join();
}

void Cashier::ProduceOrder()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(50, 500);
    while (not stop_flag_.load())
    {
        std::string order = "Cashier_" + std::to_string(cashier_id_) + " order_No: " + std::to_string(++number_of_order_);;
        std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
        manager_.ProduceOrder(order);
    }
}
