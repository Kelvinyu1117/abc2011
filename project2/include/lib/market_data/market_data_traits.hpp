#pragma once

#include "lib/market_data/md_utils.hpp"

namespace market_data {
template <typename Traits> struct MarketDataTraits {
  using PriceBook = PriceBook<Traits>;
  using Trade = Trade<Traits>;
};
} // namespace market_data
