#pragma once

#include "lib/execution/execution_event_listener.hpp"
#include "lib/simulation/exchange_simulator.hpp"
#include <memory>
#include <vector>
namespace execution {

template <typename Traits> struct SimulationExecutionContextConfig {
  std::shared_ptr<simulation::LatencyModel<Traits>> latency_model;
};

template <typename Traits>
class SimulationExecutionContext : public IExecutionEventListener<Traits> {
public:
  using IExecutionEventListener = IExecutionEventListener<Traits>;
  using ExchangeSimulator = simulation::ExchangeSimulator<Traits>;
  using OrderBook = ExchangeSimulator::OrderBook;
  using OrderID = Traits::OrderID;
  using OrderInfo = utils::Order<Traits>;
  using ExecutionInfo = ExecutionReport<Traits>;
  using Config = SimulationExecutionContextConfig<Traits>;

  SimulationExecutionContext(Config config)
      : exchange(std::make_unique<ExchangeSimulator>(
            std::move(config.latency_model), *this)) {}

  void init() {}
  void start() {}

  // these methods should be triggered by position manager/risk manager
  void send_order(OrderInfo &order) {
    exchange->on_receive_new_order(order);
    exchange->process_event();
  }

  void amend_order(OrderID order_id, OrderInfo &order) {
    exchange->on_receive_amend_order(order);
  }

  void cancel_order(OrderID order_id, OrderInfo &order) {
    exchange->on_receive_cancel_order(order_id);
  }

  void on_order_execution(OrderBook &orderbook, const OrderInfo &order_info,
                          const ExecutionInfo &executionInfo) override {}

  void on_send_order(OrderBook &orderbook,
                     const OrderInfo &order_info) override {}

  ExchangeSimulator *exchange_simulator() { return exchange.get(); }

private:
  std::vector<std::shared_ptr<IExecutionEventListener>> listener;
  std::unique_ptr<ExchangeSimulator> exchange;
};
} // namespace execution
