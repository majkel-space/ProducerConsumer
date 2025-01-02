#include <atomic>
#include <csignal>
#include <iostream>
#include <thread>
#include "warehouse.hpp"

std::atomic_bool stop_flag(false);

void SignalHandler(int signal)
{
    if (signal == SIGINT)
    {
        std::cout << "Stop signal recieved" << std::endl;
        stop_flag.store(true);
    }
}

int main()
{
    std::signal(SIGINT, SignalHandler);

    std::cout << "Hello in Warehouse\n";
    Warehouse warehouse{};
    warehouse.OpenWarehouse();
    while(not stop_flag.load())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    std::cout << "EXIT Warehouse\n";

    return 0;
}
