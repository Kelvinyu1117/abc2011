#pragma once

#include "lib/market_data/hist/hist_md_config.hpp"
#include "lib/market_data/md_event_listener.hpp"
#include "lib/market_data/md_utils.hpp"
#include "lib/simulation/latency_model.hpp"
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
 * @brief This is a actual historical market data client, expected to load the
 * market data from some static data source (database, file, ...),and parse
 * the market data, and populate the market data to the upper layer
 * One MDContext represent one data stream
 * @tparam Traits
 */
template <typename DataTraits> class HistoricalMDContext {
  using Trade = Trade<DataTraits>;
  using PriceBook = PriceBook<DataTraits>;
  using IL3EventListener = IL3EventListener<DataTraits>;
  using Order = utils::Order<DataTraits>;
  using L3MDEvent = std::variant<Trade, Order>;
  using LatencyModel = simulation::LatencyModel<DataTraits>;

public:
  explicit HistoricalMDContext(HistMDContextConfig config)
      : config(std::move(config)) {}

  void add_client(const std::shared_ptr<IL3EventListener> &client) {
    l3_clients.emplace_back(client);
  }
  void init() {}

  void start() {
    auto &clients = l3_clients;
    switch (config.data_source) {
      {
      case MDSource::DATABASE: {
        while (auto md_event = fetch_data_from_db()) {
          std::visit(
              utils::overloaded{[this](const Trade &trade) {
                                  std::ranges::for_each(
                                      clients, [&, this](const auto &client) {
                                        client->on_trade(trade);
                                      });
                                },
                                [this](const Order &order) {
                                  std::ranges::for_each(
                                      clients, [&, this](const auto &client) {
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
        break;
      }
      case MDSource::FILE: {
        while (auto md_event = fetch_data_from_file()) {
          std::visit(
              utils::overloaded{[this](const Trade &trade) {
                                  std::ranges::for_each(
                                      clients, [&, this](const auto &client) {
                                        client->on_trade(trade);
                                      });
                                },
                                [this](const Order &order) {
                                  std::ranges::for_each(
                                      clients, [&, this](const auto &client) {
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
        break;
      }
      default:
        throw std::runtime_error("Cannot find appropriate source for starting "
                                 "the historical simulation.");
      }
    }
  }

private:
  std::optional<L3MDEvent> fetch_data_from_db() {
    // assume reading the data from database
    return Trade{};
  }

  std::optional<L3MDEvent> fetch_data_from_file() {
    // assume reading the data from file
    return Trade{};
  }

private:
  std::vector<std::shared_ptr<IL3EventListener>>
      l3_clients; // shared_ptr is slow, can be improved later
  HistMDContextConfig config;
  std::shared_ptr<LatencyModel> latency_model;
};
} // namespace market_data
