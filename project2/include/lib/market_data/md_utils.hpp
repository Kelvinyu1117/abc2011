#pragma once
#include "lib/utils/pricesize.hpp"
#include <array>
#include <cstdint>
#include <limits>
#include <optional>
namespace market_data {
template <typename Traits> struct PriceBook {
  using NanoTimestamp = Traits::NanoTimestamp;
  using Instrument = Traits::Instrument;

  constexpr static size_t DEFAULT_BOOK_LEVEL = 20;

  NanoTimestamp time;
  uint64_t bid_levels;
  uint64_t ask_levels;
  Instrument instrument;
  utils::PriceSize<Traits> bids[DEFAULT_BOOK_LEVEL];
  utils::PriceSize<Traits> asks[DEFAULT_BOOK_LEVEL];
};

template <typename Traits> struct Trade {
  using NanoTimestamp = Traits::NanoTimestamp;
  using Instrument = Traits::Instrument;
  using Side = Traits::Side;

  NanoTimestamp time;
  Instrument instrument;
  utils::PriceSize<Traits> pxsz;
  std::optional<Side> side; // some market data may not have side information
};

} // namespace market_data
