#include <memory>
#include <vector>

namespace strat {

template <typename Traits> class StrategyEngine {
  using MarketDataContext = typename Traits::MarketDataContext;
  using ExecutionContext = typename Traits::ExecutionContext;

  StrategyEngine() : _md_cxt(this), _ex_cxt(this) {}

private:
  MarketDataContext _md_cxt;
  ExecutionContext _ex_cxt;
};
} // namespace strat
