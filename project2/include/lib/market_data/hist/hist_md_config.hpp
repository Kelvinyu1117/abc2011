#pragma once

#include "lib/market_data/md_context_config.hpp"
#include "lib/market_data/md_utils.hpp"
#include "lib/simulation/latency_model.hpp"
#include "lib/symbology/exchange.hpp"
#include <memory>

namespace market_data {

template <typename Traits> struct HistMDContextConfig : public MDContextConfig {
  std::shared_ptr<simulation::LatencyModel<Traits>> latency_model;
};
} // namespace market_data
