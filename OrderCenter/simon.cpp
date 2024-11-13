#include <atomic>
#include <chrono>
#include <iostream>
#include <random>
#include "simon.hpp"

Simon::Simon(const std::uint8_t simon_id)
    : simon_id_{simon_id}
{
    std::cout << "CTOR simon no " << (int)simon_id_ << std::endl;
}

void Simon::SimonJoin()
{
    if (simon_tread_.joinable())
        simon_tread_.join();
}

void Simon::ConsumeOrder()
{
    std::cout << " Simon no " << (int)simon_id_ << std::endl;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(50, 500);
    std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
}
