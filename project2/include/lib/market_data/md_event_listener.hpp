#pragma once
#include "lib/market_data/md_utils.hpp"
namespace market_data {

template <typename DataTraits> class IL3EventListener {
public:
  using Trade = DataTraits::Trade;
  using PriceBook = DataTraits::PriceBook;

  // on receive trade data from the market update
  virtual void on_trade(const Trade &trade) = 0;

  // on receive order data from the market update
  virtual void on_quote(const PriceBook &book) = 0;
};
} // namespace market_data
