#pragma once
#include <array>
#include <cstdint>
#include <limits>
#include <optional>
namespace market_data {

template <typename Traits> struct PriceSize {
  using Price = Traits::Price;
  using Size = Traits::Size;

  Price px;
  Size sz;
  bool is_valid{false};
};

template <typename Traits> struct PriceBook {
  using Instrument = Traits::Instrument;

  constexpr static size_t DEFAULT_BOOK_LEVEL = 20;

  Instrument instrument;
  uint64_t bid_levels;
  uint64_t ask_levels;
  PriceSize<Traits> bids[DEFAULT_BOOK_LEVEL];
  PriceSize<Traits> asks[DEFAULT_BOOK_LEVEL];
};

template <typename Traits> struct Trade {
  using Instrument = Traits::Instrument;
  using Side = Traits::Side;

  Instrument instrument;
  PriceSize<Traits> pxsz;
  std::optional<Side> side; // some market data may not have side information
};

enum class MDSource : int { DATABASE, FILE, LIVE };
} // namespace market_data
