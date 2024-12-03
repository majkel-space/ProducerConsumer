#include <atomic>
#include <iostream>
#include "simon.hpp"

void Simon::SimonJoin()
{
    if (simon_tread_.joinable())
        simon_tread_.join();
}

void Simon::ConsumeOrder(const std::string& order)
{
    order_.clear();
    order_ = order + " Simon no " + std::to_string(simon_id_);
    client.ConnectToServer(order_);
}
