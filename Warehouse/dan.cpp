#include <atomic>
#include <chrono>
#include <random>
#include <thread>
#include "dan.hpp"

extern std::atomic<bool> stop_flag;

void Dan::ProcessOrders()
{
    //TODO put orders to queue
    while (not stop_flag)
    {
        server_.Listen(client_orders_);
    }
}
