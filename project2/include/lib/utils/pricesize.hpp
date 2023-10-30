#pragma once

namespace utils {
template <typename Traits> struct PriceSize {
  using Price = Traits::Price;
  using Size = Traits::Size;

  Price px;
  Size sz;
  bool is_valid{false};
};
} // namespace utils
