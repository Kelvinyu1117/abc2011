#pragma once
#include "lib/execution/execution_event_listener.hpp"
#include <memory>
#include <vector>

namespace execution {
/**
 * @brief The execution context should have the 'LIVE' and 'SIM' version, for
 * 'LIVE', it should do some risk check or having some execution algo and send
 * the order to some external order entry management system (handling the
 * exchange protocol), for 'SIM', it should utilize the exchange simulator in
 * the library, and send order to the matching engine of the simulator
 *
 */
template <typename Traits> class LiveExecutionContext {
  using IExecutionEventListener = IExecutionEventListener<Traits>;
  using OrderID = Traits::OrderID;
  using OrderInfo = utils::Order<DataTraits>;
  using ExecutionInfo = ExecutionReport;

public:
  void init() {}
  void start() {
    /**
     * subscribe order update from external OMS system
     *
     */
  }

  // these methods should be triggered by position manager/risk manager
  void send_order(OrderInfo &order) {}

  void amend_order(OrderID order_id, OrderInfo &order) {}

  void cancel_order(OrderInfo &order) {}

private:
  std::vector<std::shared_ptr<IExecutionEventListener>> listener;
  // ... some object to send the order to external system
};
} // namespace execution
