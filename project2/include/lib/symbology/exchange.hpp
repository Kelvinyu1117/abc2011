#pragma once

#include <cstdint>
namespace symbology {
enum class Exchange : uint32_t {
  HK_HKEX,
  HK_HKFE,
  CN_CFFEX,
  CN_SSE,
  TW_TWSE,
  JP_TSE,
  US_CME,
  FAKE_EXCHANGE
};
}
