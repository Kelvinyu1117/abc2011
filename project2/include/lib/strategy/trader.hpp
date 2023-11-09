#pragma once

#include "lib/execution/execution_event_listener.hpp"
#include "lib/execution/execution_report.hpp"
#include "lib/execution/live_execution_context.hpp"
#include "lib/execution/sim_execution_context.hpp"
#include "lib/market_data/hist/hist_md_config.hpp"
#include "lib/market_data/hist/hist_md_context.hpp"
#include "lib/market_data/live/live_md_context.hpp"
#include "lib/market_data/md_utils.hpp"
#include "lib/strategy/signal.hpp"
#include "lib/strategy/strategy_engine.hpp"
#include "lib/traits/data_traits.hpp"
#include "lib/utils/order.hpp"
#include "lib/utils/orderbook.hpp"
#include <memory>
#include <thread>
namespace strategy {

template <typename Traits> struct LiveContext {
  using LiveMDContext = Traits::LiveMDContext;
  using LiveExecutionContext = Traits::LiveExecutionContext;

  LiveMDContext md_context;
  LiveExecutionContext execution_context;
};

template <typename Traits> struct SimulationContext {
  using SimMDContext = Traits::SimMDContext;
  using SimExecutionContext = Traits::SimExecutionContext;

  SimMDContext md_context;
  SimExecutionContext execution_context;
};

template <typename DataTraits, typename ContextTraits>
class Trader : public market_data::IL3EventListener<DataTraits>,
               public execution::IExecutionEventListener<DataTraits> {

public:
  using Trade = market_data::Trade<DataTraits>;
  using OrderBook = utils::OrderBook<DataTraits>;
  using OrderInfo = utils::Order<DataTraits>;
  using StrategyEngine = StrategyEngine<DataTraits>;
  using ExecutionInfo = execution::ExecutionReport<DataTraits>;
  using LiveContext = LiveContext<ContextTraits>;
  using SimulationContext = SimulationContext<ContextTraits>;

  Trader(LiveContext context)
      : live_context{std::make_unique<LiveContext>(std::move(context))} {}

  Trader(SimulationContext context)
      : sim_context{std::make_unique<SimulationContext>(std::move(context))} {}

  void init(std::vector<std::unique_ptr<StrategyEngine>> strategies) {
    strategies = std::move(strategies);

    if (live_context) {
      live_context->md_context.add_client(this);
    }

    if (sim_context) {
      sim_context->md_context.add_client(this);

      sim_context->md_context.add_client(
          sim_context->execution_context.exchange_simulator());
    }
  }
  void start() {
    if (live_context) {
      live_context->md_context.start();
    }

    if (sim_context) {
      sim_context->md_context.start();
    }
  }

  void on_trade(const Trade &trade) {
    std::ranges::for_each(strategies, [this, &trade](auto &strat) {
      if (!strat)
        strat->on_trade(trade);
    });
  }

  // market data event
  void on_order_add(const OrderInfo &order_info) {
    std::ranges::for_each(strategies, [this, &order_info](auto &strat) {
      if (!strat)
        strat->on_order_add(order_info);
    });
  }

  void on_order_cancel(const OrderInfo &order_info) {
    std::ranges::for_each(strategies, [this, &order_info](auto &strat) {
      if (!strat)
        strat->on_order_cancel(order_info);
    });
  }

  // execution event
  void on_order_execution(OrderBook &orderbook, const OrderInfo &order_info,
                          const ExecutionInfo &executionInfo) {
    // print something maybe?
  }

  void on_send_order(OrderBook &order, const OrderInfo &order_info) {
    // this is the feedback from the exchange saying that the order has been
    // sent, update the position manager maybe
  }

  void on_signal(Signal sig) {
    OrderInfo order;
    sim_context->execution_context.send_order(
        order); // send a order regardless the signal, just demo
  }

private:
  std::unique_ptr<LiveContext> live_context;
  std::unique_ptr<SimulationContext> sim_context;
  std::vector<std::unique_ptr<StrategyEngine>> strategies;
};
} // namespace strategy
