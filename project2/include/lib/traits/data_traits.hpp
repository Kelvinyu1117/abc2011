#pragma once

#include "lib/symbology/instrument.hpp"
#include "lib/utils/side.h"
#include <deque>
#include <map>
#include <unordered_map>

/**
 * @brief This trait allows us to configure the internal data structure easily
 *
 */
struct DataTraits {
  using Price = double; // use double for now, maybe should use integer type
  using IntPrice = uint64_t;
  using Size = uint64_t;
  using String =
      std::string; // std::string is slow, can be replaced by something better

  using NanoTimestamp = uint64_t; // it shouldn't be navie u64, can be replaced
                                  // by something better
  using Instrument = symbology::Instrument<String>;
  using Side = utils::Side;

  template <typename K, typename V, typename Comparator = std::less<>>
  using Map = std::map<K, V, Comparator>; // std::map is slow, can
                                          // be replaced by something better
  template <typename T> using Queue = std::deque<T>;
};
