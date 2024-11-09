#include <condition_variable>

class Semaphore
{
public:
    explicit Semaphore(int count = 0) : count_{count} {}

    void acquire()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_variable_.wait(lock, [&] { return count_ > 0; });
        --count_;
    }

    void release()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        ++count_;
        condition_variable_.notify_one();
    }

private:
    std::mutex mutex_;
    std::condition_variable condition_variable_;
    int count_{};
};
