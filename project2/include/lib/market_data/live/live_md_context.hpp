#pragma once
#include "lib/market_data/live/live_md_config.hpp"
#include "lib/market_data/md_event_listener.hpp"
#include "lib/utils/helpers.hpp"
#include "lib/utils/order.hpp"
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
template <typename DataTraits> class LiveMDContext {
  using Trade = Trade<DataTraits>;
  using PriceBook = PriceBook<DataTraits>;
  using IL3EventListener = IL3EventListener<DataTraits>;
  using Order = utils::Order<DataTraits>;
  using L3MDEvent = std::variant<Trade, Order>;

public:
  explicit LiveMDContext(LiveMDContextConfig config)
      : config(std::move(config)) {}

  void add_client(IL3EventListener *client) { l3_clients.emplace_back(client); }

  void init() {}

  void start() {
    if (config.data_source == MDSource::LIVE) {
      auto &clients = l3_clients;
      while (auto md_event = read_event_from_network()) {
        std::visit(
            utils::overloaded{
                [this, &clients](const Trade &trade) {
                  std::ranges::for_each(clients, [&, this](const auto &client) {
                    client->on_trade(trade);
                  });
                },
                [this, &clients](const Order &order) {
                  std::ranges::for_each(clients, [&, this](const auto &client) {
                    switch (order.action) {
                    case utils::OrderAction::ADD:
                      client->on_order_add(order);
                    case utils::OrderAction::CANCEL:
                      client->on_order_cancel(order);
                    }
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
  std::optional<L3MDEvent> read_event_from_network() {
    // assume reading the data from network
    return Trade{};
  }

private:
  std::vector<IL3EventListener *> l3_clients;
  LiveMDContextConfig config;
  // expect some member variables for network communication...
};
} // namespace market_data
