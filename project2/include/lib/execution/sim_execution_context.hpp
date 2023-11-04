#pragma once

#include "lib/execution/execution_event_listener.hpp"
#include <memory>
#include <vector>
namespace execution {

template <typename Traits> class SimulationExecutionContext {
  using IExecutionEventListener = IExecutionEventListener<Traits>;
  using OrderID = Traits::OrderID;
  using OrderInfo = utils::Order<DataTraits>;
  using ExecutionInfo = ExecutionReport;

public:
  void init() {}
  void start() {
    /**
     * subscribe order update from exchange simulator
     *
     */
  }

  // these methods should be triggered by position manager/risk manager
  void send_order(OrderInfo &order) {}

  void amend_order(OrderID order_id, OrderInfo &order) {}

  void cancel_order(OrderInfo &order) {}

private:
  std::vector<std::shared_ptr<IExecutionEventListener>> listener;
};
} // namespace execution
