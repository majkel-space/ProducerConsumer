#ifndef DELIVERYCAR
#define DELIVERYCAR

#include <cstdint>
#include <future>
#include <string>
#include "queue.hpp"

class Warehouse;

class DeliveryCar
{
  public:
    DeliveryCar(std::uint8_t);
    ~DeliveryCar();

    void StartDelivering(Warehouse&, Queue<std::future<std::string>>&);
    void DeliverOrder(const std::string&, Queue<std::future<std::string>>&);
    void Join();

  private:
    std::uint8_t id_;
    std::atomic_bool stop_flag_;
    std::thread delivery_thread_;
};

#endif //DELIVERYCAR
