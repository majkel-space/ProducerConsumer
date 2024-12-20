#include <atomic>
#include <random>
#include <thread>
#include "warehouse.hpp"

extern std::atomic<bool> stop_flag;

void Warehouse::OpenWarehouse()
{
    std::cout << "OpenWarehouse1\n";
    Dan dan{stop_flag, order_queue_, delivery_queue_};
    std::cout << "OpenWarehouse2\n";
    while (not stop_flag.load())
    {
         std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "OpenWarehouse3\n";
}
