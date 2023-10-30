#pragma once
#include "lib/market_data/live/live_md_config.hpp"
#include "lib/market_data/md_event_handler.hpp"
#include "lib/market_data/md_utils.hpp"
#include "lib/utils/helpers.hpp"
#include <algorithm>
#include <memory>
#include <optional>
#include <stdexcept>
#include <variant>
#include <vector>

namespace market_data {
/**
 * @brief This is a actual live market data client, expected to join the
 * multicast group (UDP) or connect to the market data server (TCP) or read the
 * data directly from shared memory (fastest), after that, parse the market
 * data, and populate the market data to the upper layer
 *
 * @tparam Traits
 */
template <typename Traits> class LiveMDContext {
  using IMDEventListener = IMDEventListener<Traits>;
  using Trade = Traits::Trade;
  using PriceBook = Traits::PriceBook;
  using MDEvent = std::variant<Trade, PriceBook>;

public:
  explicit LiveMDContext(LiveMDContextConfig config)
      : config(std::move(config)) {}

  void add_client(const std::shared_ptr<IMDEventListener> &client) {
    clients.emplace_back(client);
  }

  void init() {}

  void start() {
    if (config.data_source == MDSource::LIVE) {
      while (auto md_event = read_event_from_network()) {
        std::visit(
            utils::overloaded{
                [this](const Trade &trade) {
                  std::ranges::for_each(clients, [&, this](const auto &client) {
                    client->on_trade(trade);
                  });
                },
                [this](const PriceBook &book) {
                  std::ranges::for_each(clients, [&, this](const auto &client) {
                    client->on_book(book);
                  });
                }},
            *md_event);
      }
    } else {
      throw std::runtime_error("Cannot find appropriate source for starting "
                               "the live trading.");
    }
  }

private:
  std::optional<MDEvent> read_event_from_network() {
    // assume reading the data from network
    return Trade{};
  }

private:
  std::vector<std::shared_ptr<IMDEventListener>> clients;
  LiveMDContextConfig config;
  // expect some member variables for network communication...
};
} // namespace market_data
