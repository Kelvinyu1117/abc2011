#pragma once

#include "lib/market_data/md_utils.hpp"
#include "lib/symbology/exchange.hpp"

namespace market_data {
struct HistMDContextConfig {
  MDSource data_source{MDSource::FILE};
  symbology::Exchange exchange;
};
} // namespace market_data
