#pragma once
#include "lib/symbology/exchange.hpp"
#include <cstdint>
#include <string>
namespace symbology {
enum class InstrumentType : uint32_t {
  SPOT,
  BOND,
  FUTURE,
  OPTION,
};

template <typename Traits> class Instrument {
  using String = Traits::String;

  InstrumentType type;
  Exchange exchange;
  String name; 
};
} // namespace symbology
