#ifndef SIMON
#define SIMON

#include <atomic>
#include <cstdint>
#include <functional>
#include <thread>
#include "semaphore.hpp"
#include "client.hpp"

class Simon
{
  public:
    Simon(const std::uint8_t);
    Simon(const Simon&) = delete;
    Simon& operator=(const Simon&) = delete;
    Simon(Simon&&) = default;
    Simon& operator=(Simon&&) = default;
    ~Simon() = default;

    void ConsumeOrder(const std::string&);
    void SimonJoin();

  private:
    std::uint8_t simon_id_{};
    std::uint32_t number_of_order_{};
    std::thread simon_tread_{std::bind(&Simon::ConsumeOrder, this, order_)};
    std::string order_{};
    Client client;
};

#endif
