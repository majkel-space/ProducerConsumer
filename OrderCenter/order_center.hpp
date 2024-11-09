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

    std::queue<std::string>& GetOrdeeQueue() { return order_queue_; }

  private:
    void CollectOrders(const std::uint8_t);
    void PlaceOrderInQueue(std::uint8_t);

    std::vector<std::thread> cachier_treads_{};
    std::uint32_t number_of_order_{};
    std::queue<std::string> order_queue_{};
    std::mutex queue_mutex_;
    std::condition_variable queue_condition_;
    Semaphore queue_semaphore_{sem_count};
};
