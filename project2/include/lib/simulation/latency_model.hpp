#pragma once

#include <cstdint>
namespace simulation {

/**
 * @brief Provide a way to simulate the latency of the feed, order entry,
 * order response
 *
 */

template <typename TModel> class LatencyModel {
public:
protected:
  LatencyModel();
  uint64_t feed_latency_ns;
  uint64_t order_entry_ns;
  uint64_t order_response_ns;
};

struct NoLatencyModel {};

struct ConstantLatencyModel {};

} // namespace simulation
