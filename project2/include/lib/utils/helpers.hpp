#pragma once

#include <cmath>
#include <type_traits>
namespace utils {
template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};

struct MathUtilities {
  static constexpr double EPSLION = 1e-6;
  template <typename T>
  static constexpr bool equal(T a, T b, double eps = EPSLION)
    requires std::is_constructible_v<T, double>
  {
    return fabs(a - b) < eps;
  }
};

} // namespace utils
