#pragma once

#include "lib/market_data/md_event_listener.hpp"
namespace strategy {

template <typename Traits>
class StrategyEngine : public market_data::IL3EventListener<Traits> {
  using Trade = market_data::Trade<Traits>;
  using OrderInfo = utils::Order<Traits>;

public:
  void on_trade(const Trade &trade) {}

  void on_order_add(const OrderInfo &order) {}

  void on_order_cancel(const OrderInfo &order) {}
};
} // namespace strategy
