#pragma once
#include "lib/market_data/md_utils.hpp"
#include "lib/symbology/exchange.hpp"
namespace market_data {

enum class MDSource : uint16_t { DATABASE, FILE, LIVE };
enum class MDFeedType : uint16_t { L1, L2, L3 };

struct MDContextConfig {
  MDSource data_source;
  MDFeedType feed_type;
  symbology::Exchange exchange;
};
} // namespace market_data
