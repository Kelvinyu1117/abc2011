#pragma once

#include "lib/strat/ema.hpp"
#include <asm-generic/errno-base.h>
#include <stdexcept>
#include <string>
namespace strat {

template <typename Traits> struct StrategyFactory {
  using MarketDataContext = Traits::MDContext;
  using ExecutionContext = Traits::ExecutionClient;

  template <typename TConfig>
  static auto make_strategy(std::string strategy_name, TConfig &&config) {
    if (strategy_name == "ema")
      return EMAStrategy<Traits>(std::forward<TConfig>(config));
    else
      throw std::runtime_error("Cannot instantiate corresponding strategy.");
  }
};
} // namespace strat
