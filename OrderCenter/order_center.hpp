#include <queue>
#include <string>
#include <thread>
#include "semaphore.hpp"

static const std::uint8_t number_of_simons{2};

class OrderCenter
{
  public:
    OrderCenter();
    ~OrderCenter();

  private:
    void CollectOrders(const std::uint8_t);
    void PushOrderToQueue(std::uint8_t);
    void ProcessOrders(const std::uint8_t);
    void PopOrderFromQueue(const std::uint8_t);
    void ReleaseQueues();

    std::vector<std::thread> cachier_treads_{};
    std::vector<std::thread> simons_treads_{};

    std::uint32_t number_of_order_{};
    std::queue<std::string> order_queue_{};
    std::mutex queue_mutex_;
    Semaphore queue_empty_slots_{number_of_simons};
    Semaphore queue_filled_slots_{};
};
