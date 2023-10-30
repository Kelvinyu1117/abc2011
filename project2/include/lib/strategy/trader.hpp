#pragma once

#include "lib/strategy/signal.hpp"
#include <thread>
namespace strategy {

template <typename MarketDataTraits> class Trader {

  using Trade = MarketDataTraits::Trade;
  using PriceBook = MarketDataTraits::PriceBook;
  //   using OrderBook = Traits::OrderBook;
  //   using OrderInfo = Traits::OrderInfo;
  //   using ExecutionInfo = Traits::ExecutionInfo;

public:
  void run() {
    std::jthread([this]() {});
  }

  // execution/matching engine callback
  //   void on_order_add(OrderBook &order, const OrderInfo &order_info) override
  //   {}

  //   void on_order_cancel(OrderBook &orderbook,
  //                        const OrderInfo &order_info) override {}

  //   void on_order_execution(OrderBook &orderbook, const OrderInfo
  //   &order_info,
  //                           const ExecutionInfo &executionInfo) override {}

  //   // not very sure, assume it is the callback when the matching engine
  //   // receive the order
  //   void on_send_order(OrderBook &order, const OrderInfo &order_info)
  //   override {}

  void on_signal(Signal sig) {
    switch (sig) {
    case Signal::OPEN_LONG:
      break;
    case Signal::CLOSE_LONG:
      break;
    case Signal::OPEN_SHORT:
      break;
    case Signal::CLOSE_SHORT:
      break;
    default:
      break;
    };
  }

  // private:
  //   MarketDataContextKind md_context;
  //   StrategyContextKind strategy;
};
} // namespace strategy
