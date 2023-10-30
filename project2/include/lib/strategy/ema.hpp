#pragma once
#include <memory>
namespace strat {

struct EmaStrategyConfig {};

/**
 * @brief Just a demo strategy, it will calculate the ema of the trade px and
 * send order accordingly
 *
 * @tparam Traits
 */
template <typename Traits> class EMAStrategy {
  using Trade = Traits::Trade;
  using PriceBook = Traits::PriceBook;
  using OrderBook = Traits::OrderBook;
  using OrderInfo = Traits::OrderInfo;
  using ExecutionInfo = Traits::ExecutionInfo;
  using ExecutionContext = Traits::ExecutionContext;

public:
  explicit EMAStrategy(EmaStrategyConfig &config) {}

  void init() {}

  void start() {}

  // market data callback
  void on_trade(const Trade &trade) override {}

  void on_book(const PriceBook &book) override {}

private:
};
} // namespace strat
