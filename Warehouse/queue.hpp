#ifndef QUEUE
#define QUEUE

#include <mutex>
#include <optional>
#include <queue>

template <typename T>
class Queue
{
  public:
    Queue() = default;
    ~Queue() = default;

    void Push(T&& value)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(std::move(value));
    }

    std::optional<T> Pop()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty())
        {
            return std::nullopt;
        }
        T value = std::move(queue_.front());
        queue_.pop();
        return value;
    }

    bool IsEmpty()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    int Size() const
    {
        return queue_.size();
    }

  private:
    std::mutex mutex_;
    std::queue<T> queue_;
};

#endif //QUEUE
