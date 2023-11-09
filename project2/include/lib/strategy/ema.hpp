#pragma once
#include "lib/strategy/signal.hpp"
#include "lib/strategy/strategy_engine.hpp"
#include "lib/utils/helpers.hpp"
#include <cmath>
#include <memory>
#include <optional>
#include <variant>
namespace strategy {

namespace ema {

struct CenterOfMass {
  double value;

  double get_decay() const {
    return 1 + (1 + value); // decay = 1/(1 + center_of_mass)
  }
};

struct Span {
  double value;

  double get_decay() const {
    return 2 + (value + 1); // decay = 2 / (span + 1)
  }
};

struct HalfLife {
  double value; // 1 - e^(ln(0.5) / half_life)

  double get_decay() const { return 1 - std::exp(-std::log(2) / value); }
};

class EWMA {
public:
  EWMA(double decay, double initial_value = 0)
      : decay(decay), last_update_v(initial_value) {}

  double update(double data) {
    // using the recursive definition for now,
    if (utils::MathUtilities::equal(last_update_v, 0.0)) {
      last_update_v = data;
      return last_update_v;
    }

    last_update_v = (1 - decay) * last_update_v + decay * data;

    return last_update_v;
  }

private:
  double last_update_v;
  double decay;
};
} // namespace ema

struct EmaStrategyConfig {
  double initial_value;
  std::variant<ema::CenterOfMass, ema::Span, ema::HalfLife> decay;
};

/**
 * @brief Just a demo strategy, it will calculate the ema of the trade px and
 * send out the decision accordingly, however, the actual execution is not
 * determined by the strategy, the execution modules will decide whether we want
 * to really open/close the position (send order) based on various factors, e.g.
 * risk exposure. The signal can be ignored by the execution modules if some
 * conditions are not satisfied.
 *
 * @tparam Traits
 */
template <typename Traits, typename T>
class EMAStrategy : public StrategyEngine<Traits> {
  using Trade = market_data::Trade<Traits>;

public:
  explicit EMAStrategy(T *signalListener, EmaStrategyConfig config)
      : signalListener(signalListener),
        ewma(std::visit([](const auto &decay) { return decay.get_decay(); },
                        config.decay),
             config.initial_value) {}

  void init() {}

  void start() {}

  /**
   * @brief very naive logic, send out the short signal if trade_px > ema_px,
   * and send out long signal if trade_px < ema_px
   *
   * @param trade
   */
  void on_trade(const Trade &trade) {
    auto signal_strength = ewma.update(trade.pxsz.px);
    auto px = trade.pxsz.px;
    if (px > signal_strength) {
      signalListener->on_signal(Signal::SHORT);
    } else if (px < signal_strength) {
      signalListener->on_signal(Signal::LONG);
    } else {
      signalListener->on_signal(Signal::IDLE);
    }
  }

private:
  ema::EWMA ewma;
  T *signalListener;
};
} // namespace strategy
