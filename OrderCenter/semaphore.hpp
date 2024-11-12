#include <condition_variable>

class Semaphore
{
public:
    explicit Semaphore(int count = 0) : count_{count} {}
    ~Semaphore()
    {
        std::cout << "Semaphore DTOR\n";
        std::unique_lock<std::mutex> lock(mutex_);
        std::cout << "Semaphore DTOR locked\n";
        condition_variable_.notify_all();
        std::cout << "Semaphore DTOR notified\n";
    }

    bool acquire(const std::atomic<bool>& stop_flag)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_variable_.wait(lock, [&] { return count_ > 0 or stop_flag.load(); });
        if (stop_flag.load())
            return false;
        --count_;
        return true;
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
