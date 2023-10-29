#pragma once
#include "lib/symbology/exchange.hpp"
#include <cstdint>
#include <string>
namespace symbology {
enum class InstrumentType : uint32_t {
  STOCK,
  BOND,
  FUTURE,
  OPTION,
};

class Instrument {
  InstrumentType type;
  Exchange exchange;
  std::string name; // std::string is slow, can be improved later
};
} // namespace symbology
