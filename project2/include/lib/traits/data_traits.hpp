#pragma once

#include "lib/symbology/instrument.hpp"
#include <unordered_map>

/**
 * @brief This trait allows us to configure the internal data structure easily
 *
 */
struct DataTraits {
  using Price = double; // use double for now, maybe should use integer type
  using Size = uint64_t;
  using String =
      std::string; // std::string is slow, can be replaced by something better
  using Instrument = symbology::Instrument<DataTraits>;

  template <typename K, typename V, typename Comparator>
  using UnorderedMap = std::unordered_map<K, V, Comparator>;
};
