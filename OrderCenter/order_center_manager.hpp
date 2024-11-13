#ifndef MANAGER
#define MANAGER

#include <list>

#include "cashier.hpp"
#include "semaphore.hpp"
#include "simon.hpp"

static const std::uint8_t number_of_simons{2};

template <typename T>
class OrderCenterManager
{
  public:
    OrderCenterManager(std::list<T>&& simons)
        : simons_{std::move(simons)}
    {
        InitializeSimons();
    }

    T GetFreeSimon()
    {
        free_simons_.acquire();
        std::unique_lock<std::mutex> lock(mutex_);

        if (simons_.empty())
        {
            throw std::runtime_error("Error: no free Simons");
        }

        T simon = std::move(simons_.front());
        simons_.pop_front();
        lock.unlock();
        occupied_simons_.release();
        return std::move(simon);
    }

    void ReleaseSimon(T&& simon)
    {
        occupied_simons_.acquire();
        std::unique_lock<std::mutex> lock(mutex_);
        simons_.emplace_back(std::move(simon));
        lock.unlock();
        free_simons_.release();
    }

  private:
    void InitializeSimons()
    {
        for (std::uint8_t it = 0; it < number_of_simons; ++it)
        {
            simons_.emplace_back(std::uint8_t(it + 1));
        }
    }

    std::list<T> simons_;
    std::mutex mutex_;
    Semaphore free_simons_{number_of_simons};
    Semaphore occupied_simons_{};
};

#endif
