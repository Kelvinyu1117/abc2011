#pragma once

#include "lib/market_data/market_data_traits.hpp"
#include "lib/traits/data_traits.hpp"
#include "lib/traits/production_traits.hpp"
#include "lib/traits/simulation_traits.hpp"

namespace traits {

struct AppTraits {
  using DataTraits = DataTraits;
  using MarketDataTraits = market_data::MarketDataTraits<DataTraits>;
  using ProductionTraits = ProductionTraits;
  using SimulationTraits = SimulationTraits;
};
} // namespace traits
