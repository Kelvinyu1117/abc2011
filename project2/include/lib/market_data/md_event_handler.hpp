#pragma once
#include "lib/market_data/md_utils.hpp"
namespace market_data {

template <typename Traits> class IMDEventListener {
public:
  using Trade = Traits::Trade;
  using PriceBook = Traits::PriceBook;

  // on receive trade data from the market update
  virtual void on_trade(const Trade &trade) = 0;

  // on receive book data from the market update
  virtual void on_book(const PriceBook &book) = 0;
};
} // namespace market_data
