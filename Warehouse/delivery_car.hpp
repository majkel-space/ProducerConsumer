#ifndef DELIVERYCAR
#define DELIVERYCAR

#include <cstdint>
#include <future>
#include <string>
#include "queue.hpp"

class Warehouse;
struct Order;

class DeliveryCar
{
  public:
    DeliveryCar(std::uint8_t);
    ~DeliveryCar();

    void DeliverOrder(Order&, std::promise<Order>&);
    void Join();

  private:
    std::uint8_t id_;
    std::atomic_bool stop_flag_;
    std::thread delivery_thread_;
};

#endif //DELIVERYCAR
