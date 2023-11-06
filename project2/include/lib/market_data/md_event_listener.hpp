#pragma once
#include "lib/market_data/md_utils.hpp"
#include "lib/utils/order.hpp"
namespace market_data {

template <typename DataTraits> class IL3EventListener {
public:
  using Trade = DataTraits::Trade;
  using OrderInfo = utils::Order<DataTraits>;
  using PriceBook = DataTraits::PriceBook;

  // on receive trade data from the market update
  virtual void on_trade(const Trade &trade) = 0;

  virtual void on_order_add(const OrderInfo &order) = 0;

  virtual void on_order_cancel(const OrderInfo &order) = 0;
};
} // namespace market_data
