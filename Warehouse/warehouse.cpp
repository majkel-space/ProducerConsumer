#include <atomic>
#include <random>
#include <thread>
#include "warehouse.hpp"

extern std::atomic<bool> stop_flag;
static constexpr std::uint8_t number_of_delivery_cars{3U};

void Warehouse::OpenWarehouse()
{
    Dan dan{stop_flag, order_queue_, delivery_queue_};
    for (auto it = 0U; it < number_of_delivery_cars; ++it)
    {
        DeliveryCar delivery_car(it + 1, stop_flag, order_queue_, delivery_queue_);
    }
    while (not stop_flag.load())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "OpenWarehouse END\n";
}
