#pragma once

namespace strategy {
enum class Signal { OPEN_LONG, OPEN_SHORT, CLOSE_LONG, CLOSE_SHORT };

class ISignalListener {
  virtual void on_signal(Signal signal) = 0;
};
} // namespace strategy
