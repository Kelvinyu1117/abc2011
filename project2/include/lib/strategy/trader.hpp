#pragma once

#include "lib/execution/execution_report.hpp"
#include "lib/execution/live_execution_context.hpp"
#include "lib/execution/sim_execution_context.hpp"
#include "lib/market_data/hist/hist_md_config.hpp"
#include "lib/market_data/hist/hist_md_context.hpp"
#include "lib/market_data/live/live_md_context.hpp"
#include "lib/market_data/md_utils.hpp"
#include "lib/strategy/signal.hpp"
#include "lib/utils/order.hpp"
#include "lib/utils/orderbook.hpp"
#include <thread>
namespace strategy {

template <typename Traits> struct LiveContext {
  using LiveMDContext = Traits::LiveMDContext;
  using LiveExecutionContext = Traits::LiveExecutionContex;

  std::unique_ptr<LiveMDContext> md_context;
  std::unique_ptr<LiveExecutionContext> execution_context;
};

template <typename Traits> struct SimulationContext {
  using SimMDContext = Traits::HistoricalMDContext;
  using SimExecutionContext = Traits::SimulationExecutionContext;

  std::unique_ptr<SimMDContext> md_context;
  std::unique_ptr<SimExecutionContext> execution_context;
};

template <typename DataTraits, typename ContextTraits> class Trader {

  using Trade = market_data::Trade<DataTraits>;
  using OrderBook = utils::OrderBook<DataTraits>;
  using OrderInfo = utils::Order<DataTraits>;
  using ExecutionInfo = execution::ExecutionReport<DataTraits>;
  using LiveContext = LiveContext<ContextTraits>;
  using SimulationContext = SimulationContext<ContextTraits>;

public:
  Trader(LiveContext context) : live_context(std::move(context)) {}

  Trader(SimulationContext context) : sim_context(std::move(context)) {}

  void run() {}

  // market data event
  void on_order_add(OrderBook &order, const OrderInfo &order_info) {}

  void on_order_cancel(OrderBook &orderbook, const OrderInfo &order_info) {}

  // execution event
  void on_order_execution(OrderBook &orderbook, const OrderInfo &order_info,
                          const ExecutionInfo &executionInfo) {}

  void on_send_order(OrderBook &order, const OrderInfo &order_info) {}

  void on_signal(Signal sig) {}

private:
  LiveContext live_context;
  SimulationContext sim_context;
};
} // namespace strategy
