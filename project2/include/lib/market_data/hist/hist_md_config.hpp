#pragma once

#include "lib/market_data/md_utils.hpp"

namespace market_data {
struct HistMDContextConfig {
  MDSource data_source{MDSource::FILE};
};
} // namespace market_data
