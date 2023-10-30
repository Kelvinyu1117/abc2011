#pragma once

#include "lib/market_data/hist/hist_md_config.hpp"
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
 * @brief This is a actual historical market data client, expected to load the
 * market data from some static data source (database, file, ...),and parse
 * the market data, and populate the market data to the upper layer
 * One MDContext represent one data stream
 * @tparam Traits
 */
template <typename Traits> class HistoricalMDContext {
  using Trade = Traits::Trade;
  using PriceBook = Traits::PriceBook;
  using IMDEventListener = IMDEventListener<Traits>;
  using MDEvent = std::variant<Trade, PriceBook>;

public:
  explicit HistoricalMDContext(HistMDContextConfig config)
      : config(std::move(config)) {}

  void add_client(const std::shared_ptr<IMDEventListener> &client) {
    clients.emplace_back(client);
  }
  void init() {}

  void start() {
    switch (config.data_source) {
      {
      case MDSource::DATABASE: {
        while (auto md_event = fetch_data_from_db()) {
          std::visit(utils::overloaded{
                         [this](const Trade &trade) {
                           std::ranges::for_each(clients,
                                                 [&, this](const auto &client) {
                                                   client->on_trade(trade);
                                                 });
                         },
                         [this](const PriceBook &book) {
                           std::ranges::for_each(clients,
                                                 [&, this](const auto &client) {
                                                   client->on_book(book);
                                                 });
                         }},
                     *md_event);
        }
        break;
      }
      case MDSource::FILE: {
        while (auto md_event = fetch_data_from_file()) {
          std::visit(utils::overloaded{
                         [this](const Trade &trade) {
                           std::ranges::for_each(clients,
                                                 [&, this](const auto &client) {

                                                 });
                         },
                         [this](const PriceBook &book) {
                           std::ranges::for_each(clients,
                                                 [&, this](const auto &client) {
                                                   client->on_book(book);
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
  std::optional<MDEvent> fetch_data_from_db() {
    // assume reading the data from database
    return Trade{};
  }

  std::optional<MDEvent> fetch_data_from_file() {
    // assume reading the data from file
    return Trade{};
  }

private:
  std::vector<std::shared_ptr<IMDEventListener>>
      clients; // shared_ptr is slow, can be improved later
  HistMDContextConfig config;
};
} // namespace market_data
