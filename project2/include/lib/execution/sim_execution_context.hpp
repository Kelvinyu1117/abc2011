#pragma once

#include "lib/execution/execution_event_listener.hpp"
#include "lib/simulation/exchange_simulator.hpp"
#include <memory>
#include <vector>
namespace execution {

struct SimulationExecutionContextConfig {
  simulation::ExchangeSimulatorConfig config;
};

template <typename Traits> class SimulationExecutionContext {
  using IExecutionEventListener = IExecutionEventListener<Traits>;
  using ExchangeSimulator = simulation::ExchangeSimulator<Traits>;
  using OrderID = Traits::OrderID;
  using OrderInfo = utils::Order<Traits>;
  using ExecutionInfo = ExecutionReport<Traits>;

public:
  SimulationExecutionContext(SimulationExecutionContextConfig &&config)
      : exchange{this} {}
  void init() {}
  void start() {
    /**
     * subscribe order update from exchange simulator
     *
     */
  }

  // these methods should be triggered by position manager/risk manager
  void send_order(OrderInfo &order) { exchange->on_receive_new_order(order); }

  void amend_order(OrderID order_id, OrderInfo &order) {
    exchange->on_receive_amend_order(order);
  }

  void cancel_order(OrderID order_id, OrderInfo &order) {
    exchange->on_receive_cancel_order(order_id);
  }

private:
  std::vector<std::shared_ptr<IExecutionEventListener>> listener;
  std::shared_ptr<ExchangeSimulator> exchange;
};
} // namespace execution
