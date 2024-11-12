#include <atomic>
#include <csignal>
#include <iostream>
#include "order_center.hpp"

std::atomic<bool> stop_flag(false);

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

    std::cout << "Hello in OrderCenter\n";
    OrderCenter oc;
    while(not stop_flag.load())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    std::cout << "EXIT\n";

    return 0;
}
