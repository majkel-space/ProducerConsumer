#include <atomic>
#include <functional>
#include <iostream>
#include <numeric>
#include "order_center.hpp"

extern std::atomic_bool stop_flag;
std::atomic_bool cachiers_done{false};
static const std::uint8_t number_of_cashiers{5};

OrderCenter::OrderCenter()
    : manager_{std::move(simons_)}
{
    stop_flag.store(false);
    OpenShop();
}

OrderCenter::~OrderCenter()
{
    stop_flag.store(true);
    cashiers_.clear();
    simons_.clear();
}

void OrderCenter::OpenShop()
{
    for (std::uint8_t it = 0; it < number_of_cashiers; ++it)
    {
        cashiers_.emplace_back(std::uint8_t(it + 1), stop_flag, manager_);
    }

}
