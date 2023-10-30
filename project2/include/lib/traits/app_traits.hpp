#pragma once

#include "lib/traits/data_traits.hpp"
#include "lib/traits/production_traits.hpp"
#include "lib/traits/simulation_traits.hpp"

namespace traits {

struct AppTraits {
  using DataTraits = DataTraits;
  using ProductionTraits = ProductionTraits;
  using SimulationTraits = SimulationTraits;
};
} // namespace traits
