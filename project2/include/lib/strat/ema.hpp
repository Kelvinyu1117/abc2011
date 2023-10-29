#pragma once
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
  using MarketDataContext = Traits::MDContext;
  using OrderBook = Traits::OrderBook;
  using OrderInfo = Traits::OrderInfo;
  using ExecutionInfo = Traits::ExecutionInfo;
  using ExecutionContext = Traits::ExecutionClient;

public:
  explicit EMAStrategy(MarketDataContext &market_data_context,
                       ExecutionContext &executionClient,
                       EmaStrategyConfig &config)
      : market_data_context{this}, {}

  void init() {
    market_data_context.init();
    execution_context.init();
  }

  void start() { market_data_context.start(); }

  // market data callback
  void on_trade(const Trade &trade) override {}

  void on_book(const PriceBook &book) override {}

  // execution/matching engine callback
  void on_order_add(OrderBook &order, const OrderInfo &order_info) override {}

  void on_order_cancel(OrderBook &orderbook,
                       const OrderInfo &order_info) override {}

  void on_order_execution(OrderBook &orderbook, const OrderInfo &order_info,
                          const ExecutionInfo &executionInfo) override {}

  // not very sure, assume it is the callback when the matching engine
  // receive the order
  void on_send_order(OrderBook &order, const OrderInfo &order_info) override {}

  void send_order() {
    OrderInfo order{};
    execution_context.send(order);
  }

private:
  MarketDataContext &market_data_context;
  ExecutionContext &execution_context;
};
} // namespace strat
