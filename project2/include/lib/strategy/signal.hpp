#pragma once

namespace strategy {
enum class Signal { LONG, SHORT, IDLE };

class ISignalListener {
public:
  virtual void on_signal(Signal signal) = 0;
};
} // namespace strategy
