#pragma once
#include "lib/market_data/md_utils.hpp"
namespace market_data {

struct LiveMDContextConfig {
  MDSource data_source{MDSource::LIVE};
};
} // namespace market_data
