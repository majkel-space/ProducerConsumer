#ifndef SIMON
#define SIMON

#include <atomic>
#include <cstdint>
#include "client.hpp"

class Simon
{
  public:
    Simon() = default;
    ~Simon() = default;

    void ConsumeOrder(const std::string&);
    void SetId(const std::uint8_t id) { simon_id_ = id; }

    std::atomic_bool is_occupied_{false};

  private:
    std::uint8_t simon_id_{};
    std::uint32_t number_of_order_{};
    Client client{};
};

#endif
