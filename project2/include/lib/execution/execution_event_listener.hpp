#pragma once
#include "lib/execution/execution_report.hpp"
#include "lib/utils/orderbook.hpp"
namespace execution {

template <typename Traits> class IExecutionEventListener {
  using OrderBook = utils::OrderBook<Traits>;
  using OrderInfo = utils::Order<Traits>;
  using ExecutionInfo = ExecutionReport<Traits>;

public:
  virtual void on_order_execution(OrderBook &orderbook,
                                  const OrderInfo &order_info,
                                  const ExecutionInfo &executionInfo) = 0;

  virtual void on_send_order(OrderBook &orderbook,
                             const OrderInfo &order_info) = 0;

  virtual ~IExecutionEventListener() {}
};
} // namespace execution
