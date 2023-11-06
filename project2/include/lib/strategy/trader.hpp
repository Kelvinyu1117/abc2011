#pragma once

#include "lib/execution/execution_report.hpp"
#include "lib/market_data/md_utils.hpp"
#include "lib/strategy/signal.hpp"
#include "lib/utils/order.hpp"
#include "lib/utils/orderbook.hpp"
#include <thread>
namespace strategy {

template <typename DataTraits> class Trader {

  using Trade = market_data::Trade<DataTraits>;
  using OrderBook = utils::OrderBook<DataTraits>;
  using OrderInfo = utils::Order<DataTraits>;
  using ExecutionInfo = execution::ExecutionReport<DataTraits>;

public:
  void run() {}

  // market data event
  void on_order_add(OrderBook &order, const OrderInfo &order_info) override {}

  void on_order_cancel(OrderBook &orderbook,
                       const OrderInfo &order_info) override {}

  // execution event
  void on_order_execution(OrderBook &orderbook, const OrderInfo &order_info,
                          const ExecutionInfo &executionInfo) override {}

  void on_send_order(OrderBook &order, const OrderInfo &order_info) {}

  void on_signal(Signal sig) {}
};
} // namespace strategy
