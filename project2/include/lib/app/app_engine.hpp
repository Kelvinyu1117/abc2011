#pragma once

#include "lib/app/app_config.hpp"
#include "lib/strategy/trader.hpp"
#include "lib/traits/production_traits.hpp"

#include <iostream>
#include <memory>
#include <vector>

namespace app {

enum class LaunchMode { LIVE, SIMULATION };

template <typename AppTraits> class AppEngine {
  using DataTraits = typename AppTraits::DataTraits;
  using ProductionTraits = typename AppTraits::ProductionTraits;
  using Simulation = typename AppTraits::SimulationTraits;
  using Trader = strategy::Trader<typename AppTraits::MarketDataTraits>;

public:
  explicit AppEngine(std::unique_ptr<LiveAppConfig> config)
      : mode(LaunchMode::LIVE), live_config(std::move(config)) {}

  explicit AppEngine(std::unique_ptr<SimAppConfig> config)
      : mode(LaunchMode::SIMULATION), sim_config(std::move(config)) {}

  void init() { std::cout << "AppEngine::init()\n"; }
  void start() {}

private:
  LaunchMode mode;
  std::unique_ptr<LiveAppConfig> live_config;
  std::unique_ptr<SimAppConfig> sim_config;
  std::vector<std::unique_ptr<Trader>> traders;
};
} // namespace app
