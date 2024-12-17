#include <atomic>
#include "dan.hpp"

extern std::atomic<bool> stop_flag;

Dan::Dan()
    : server_{queue_}
{}

void Dan::ProcessOrders()
{
    while (not stop_flag)
    {
        server_.Listen();
    }
}
