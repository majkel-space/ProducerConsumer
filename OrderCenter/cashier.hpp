#ifndef CASHIER
#define CASHIER

#include <cstdint>
#include <queue>
#include <thread>
#include "order_center_manager.hpp"
#include "semaphore.hpp"

class Cashier
{
  public:
    Cashier(const std::uint8_t, std::atomic_bool&, OrderCenterManager<Simon>&);
    ~Cashier();

  private:
    void ProduceOrder();

    std::uint8_t cashier_id_{};
    std::atomic_bool& stop_flag_;
    std::uint32_t number_of_order_{};
    OrderCenterManager<Simon>& manager_;
    std::thread cashier_tread_{};
};

#endif
