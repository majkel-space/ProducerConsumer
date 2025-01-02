#include <cstdint>
#include <future>
#include <string>
#include "queue.hpp"

class DeliveryCar
{
  public:
    DeliveryCar(std::uint8_t, std::atomic_bool&, Queue<std::string>&, Queue<std::future<std::string>>&);
    ~DeliveryCar();

    void StartDelivering(Queue<std::string>&, Queue<std::future<std::string>>&);
    void DeliverOrder(const std::string&, Queue<std::future<std::string>>&);

  private:
    std::uint8_t id_;
    std::atomic_bool& stop_flag_;
    std::thread delivery_thread_{};
};
