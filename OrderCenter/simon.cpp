#include <iostream>
#include "simon.hpp"

void Simon::ConsumeOrder(const std::string& order)
{
    const std::string order_msg = order + " Simon no " + std::to_string(simon_id_);
    client.PlaceOrder(order);
}
