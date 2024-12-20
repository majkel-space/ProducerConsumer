#include <cstdint>
#include <future>
#include <string>
#include "queue.hpp"

class DeliveryCar
{
  public:
    DeliveryCar(std::uint8_t);

    void StartDelivering(Queue<std::string>&, Queue<std::future<std::string>>&);
    void DeliverOrder(const std::string&, Queue<std::future<std::string>>&);

  private:
    std::uint8_t id_;
};
