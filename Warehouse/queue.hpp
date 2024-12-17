#ifndef QUEUE
#define QUEUE

#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

template <typename T>
class Queue
{
  public:
    Queue() = default;
    ~Queue() = default;

    void Push(const T& value)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(value);
        cv_.notify_one();
    }

    std::optional<T> Pop()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty())
        {
            return std::nullopt;
        }
        T value = queue_.front();
        queue_.pop();
        return value;
    }

    T WaitPop()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        cv_.wait(lock, [this]() { return !queue_.empty(); });
        T value = queue_.front();
        queue_.pop();
        return value;
    }

    bool IsEmpty() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

  private:
    std::mutex mutex_;
    std::queue<T> queue_;
    std::condition_variable cv_;
};

#endif //QUEUE
