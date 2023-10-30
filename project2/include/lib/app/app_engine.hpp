#pragma once

#include "lib/app/app_config.hpp"
#include "lib/market_data/hist/hist_md_context.hpp"
#include "lib/market_data/live/live_md_context.hpp"
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

  using HistoricalMDContext = market_data::HistoricalMDContext<DataTraits>;
  using LiveMDContext = market_data::LiveMDContext<DataTraits>;
  using Trader = strategy::Trader<typename AppTraits::MarketDataTraits>;

public:
  explicit AppEngine(std::unique_ptr<LiveAppConfig> config)
      : mode(LaunchMode::LIVE), live_config(std::move(config)) {}

  explicit AppEngine(std::unique_ptr<SimAppConfig> config)
      : mode(LaunchMode::SIMULATION), sim_config(std::move(config)) {}

  void init() { std::cout << "AppEngine::init()\n"; }
  void start() {
    /**
     * launch all traders, in term of multiple threads, target is to archive
     * thread per core architecture, and minimize the data sharing across thread
     *
     */
  }

private:
  LaunchMode mode;
  std::unique_ptr<LiveAppConfig> live_config;
  std::unique_ptr<SimAppConfig> sim_config;
  std::vector<std::unique_ptr<Trader>> traders;
};
} // namespace app
