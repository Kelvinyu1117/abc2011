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

template <typename TString> class Instrument {
  InstrumentType type;
  Exchange exchange;
  TString name;
};
} // namespace symbology
