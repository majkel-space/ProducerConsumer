#include <list>
#include <string>
#include <thread>
#include "cashier.hpp"
#include "order_center_manager.hpp"
#include "simon.hpp"

class OrderCenter
{
  public:
    OrderCenter();
    ~OrderCenter();

  private:
    void OpenShop();
    void CloseShop();

    std::list<Cashier> cashiers_{};
    OrderCenterManager<Simon> manager_;
};
