#include <queue>
#include <string>
#include <thread>
#include "semaphore.hpp"

const std::uint16_t MAX_QUEUE_SIZE{500};
const std::uint8_t sem_count{1};

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

    std::vector<std::thread> cachier_treads_{};
    std::vector<std::thread> simons_treads_{};

    std::uint32_t number_of_order_{};
    std::queue<std::string> order_queue_{};
    std::mutex queue_mutex_;
    Semaphore queue_empty_slots_{MAX_QUEUE_SIZE};
    Semaphore queue_filled_slots_{};
};
