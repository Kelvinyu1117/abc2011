#pragma once
#include "lib/execution/execution_report.hpp"
#include "lib/utils/orderbook.hpp"
namespace execution {

template <typename DataTraits> class IExecutionEventHandler {
  using OrderBook = utils::OrderBook<DataTraits>;
  using OrderInfo = utils::Order<DataTraits>;
  using ExecutionInfo = ExecutionReport;

public:
  virtual void
  on_order_execution(OrderBook &orderbook, const OrderInfo &order_info,
                     const ExecutionInfo &executionInfo) override = 0;

  virtual void on_send_order(OrderBook &order, const OrderInfo &order_info) = 0;
};
} // namespace execution
