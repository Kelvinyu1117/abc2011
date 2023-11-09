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
public:
  using Trade = Trade<DataTraits>;
  using PriceBook = PriceBook<DataTraits>;
  using IL3EventListener = IL3EventListener<DataTraits>;
  using Order = utils::Order<DataTraits>;
  using L3MDEvent = std::variant<Trade, Order>;
  using Config = HistMDContextConfig<DataTraits>;

  explicit HistoricalMDContext(Config config) : config(std::move(config)) {}

  void add_client(IL3EventListener *client) {
    if (!client)
      throw std::runtime_error("Client cannot be null.");
    l3_clients.emplace_back(client);
  }

  void init() {}

  void start() {
    auto &clients = l3_clients;
    switch (config.data_source) {
      {
      case MDSource::DATABASE: {
        while (auto md_event = fetch_data_from_db()) {
          std::visit(utils::overloaded{
                         [this, &clients](Trade &trade) {
                           std::ranges::for_each(
                               clients, [&, this](const auto client) {
                                 if (!client)
                                   return;
                                 trade.time +=
                                     config.latency_model->get_feed_latency();
                                 client->on_trade(trade);
                               });
                         },
                         [this, &clients](Order &order) {
                           std::ranges::for_each(
                               clients, [&, this](const auto client) {
                                 if (!client)
                                   return;
                                 order.time +=
                                     config.latency_model->get_feed_latency();

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
          std::visit(utils::overloaded{
                         [this, &clients](Trade &trade) {
                           std::ranges::for_each(
                               clients, [&, this](const auto client) {
                                 if (!client)
                                   return;

                                 trade.time +=
                                     config.latency_model->get_feed_latency();

                                 client->on_trade(trade);
                               });
                         },
                         [this, &clients](Order &order) {
                           std::ranges::for_each(
                               clients, [&, this](const auto client) {
                                 if (!client)
                                   return;

                                 order.time +=
                                     config.latency_model->get_feed_latency();

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
  std::vector<IL3EventListener *>
      l3_clients; // shared_ptr is slow, can be improved later
  Config config;
};
} // namespace market_data
