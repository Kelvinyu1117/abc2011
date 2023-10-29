#pragma once

#include <cstdint>
namespace symbology {
enum class Exchange : uint32_t {
  HK_HKEX,
  HK_HKFE,
  CN_CFFEX,
  CN_SHFE,
  US_CME,
  TW_TWSE
};
}
