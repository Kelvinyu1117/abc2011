#pragma once

#include "lib/app/app_config.hpp"
#include "lib/market_data/hist/hist_md_context.hpp"
#include "lib/market_data/live/live_md_context.hpp"
#include "lib/market_data/md_context_config.hpp"
#include "lib/simulation/latency_model.hpp"
#include "lib/strategy/ema.hpp"
#include "lib/strategy/trader.hpp"

#include "lib/traits/data_traits.hpp"
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

  struct ContextTraits {
    using SimMDContext = market_data::HistoricalMDContext<DataTraits>;
    using SimMDContextConfig = SimMDContext::Config;
    using SimExecutionContext =
        execution::SimulationExecutionContext<DataTraits>;
    using SimExecutionContextConfig = SimExecutionContext::Config;

    using LiveMDContext = market_data::LiveMDContext<DataTraits>;
    using LiveExecutionContext = execution::LiveExecutionContext<DataTraits>;
  };

  using Trader = strategy::Trader<DataTraits, ContextTraits>;
  using StrategyEngine = strategy::StrategyEngine<DataTraits>;

public:
  explicit AppEngine(std::unique_ptr<LiveAppConfig> config)
      : mode(LaunchMode::LIVE), live_config(std::move(config)) {}

  explicit AppEngine(std::unique_ptr<SimAppConfig> config)
      : mode(LaunchMode::SIMULATION), sim_config(std::move(config)) {

    std::shared_ptr<simulation::LatencyModel<DataTraits>> latency_model{
        std::make_shared<simulation::NoLatencyModel<DataTraits>>()};

    typename ContextTraits::SimMDContextConfig md_config;
    md_config.data_source = market_data::MDSource::DATABASE;
    md_config.feed_type = market_data::MDFeedType::L3;
    md_config.exchange = symbology::Exchange::FAKE_EXCHANGE;
    md_config.latency_model = latency_model;

    typename ContextTraits::SimExecutionContextConfig execution_config;
    execution_config.latency_model = latency_model;

    auto trader = std::make_unique<Trader>(typename Trader::SimulationContext{
        typename ContextTraits::SimMDContext{std::move(md_config)},
        typename ContextTraits::SimExecutionContext{
            std::move(execution_config)}});

    std::vector<std::unique_ptr<typename Trader::StrategyEngine>> engines;

    engines.emplace_back(
        std::make_unique<strategy::EMAStrategy<DataTraits, Trader>>(
            trader.get(),
            strategy::EmaStrategyConfig{0, strategy::ema::Span{100}}));

    trader->init(std::move(engines));
    traders.emplace_back(std::move(trader));
  }

  void init() { std::cout << "AppEngine::init()\n"; }
  void start() {
    /**
     * launch all traders, expected to be multiple threads, target is to archive
     * thread per core architecture, and minimize the data sharing across
     * thread, but for demo, just call the function
     *
     */

    std::ranges::for_each(traders, [this](auto &trader) { trader->start(); });
  }

private:
  LaunchMode mode;
  std::unique_ptr<LiveAppConfig> live_config;
  std::unique_ptr<SimAppConfig> sim_config;
  std::vector<std::unique_ptr<Trader>> traders;
};
} // namespace app
