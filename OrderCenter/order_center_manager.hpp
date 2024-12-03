#ifndef MANAGER
#define MANAGER

#include <array>

#include "cashier.hpp"
#include "semaphore.hpp"
#include "simon.hpp"

static const std::uint8_t number_of_simons{2};

template <typename T>
class OrderCenterManager
{
  public:
    OrderCenterManager()
    {
        SetSimonsData();
    }

    T& GetFreeSimon()
    {
        free_simons_.acquire();
        std::unique_lock<std::mutex> lock(mutex_);

        for (auto& simon : simons_)
        {
            if (not simon.is_occupied_)
            {
                lock.unlock();
                simon.is_occupied_.store(true);
                occupied_simons_.release();
                return simon;
            }
        }
        throw std::runtime_error("Error: no free Simons");
    }

    void ReleaseSimon(T& simon)
    {
        occupied_simons_.acquire();
        std::unique_lock<std::mutex> lock(mutex_);
        lock.unlock();
        simon.is_occupied_.store(false);
        free_simons_.release();
    }

  private:
    void SetSimonsData()
    {
        for (std::uint8_t it = 0; it < number_of_simons; ++it)
        {
            simons_.at(it).SetId(it + 1);
        }
    }

    std::array<T, number_of_simons> simons_{};
    std::mutex mutex_;
    Semaphore free_simons_{number_of_simons};
    Semaphore occupied_simons_{};
};

#endif
