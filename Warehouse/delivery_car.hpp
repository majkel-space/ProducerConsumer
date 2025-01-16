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
    DeliveryCar(std::uint8_t, Warehouse&, std::atomic_bool&);
    ~DeliveryCar();

  private:
    void StartDelivering();
    void DeliverOrder(Order&);
    std::uint8_t id_;
    Warehouse& warehouse_;
    std::atomic_bool& stop_flag_;
    std::condition_variable& cv_;
    std::thread delivery_thread_;
    std::mutex mutex_;
};

#endif //DELIVERYCAR
