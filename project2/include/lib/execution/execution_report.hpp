#pragma once

namespace execution {

/**
 * It's expected to store the result of the execution, fill_px, ..., and we can
 * use this information for post trade analysis,
 *
 */

template <typename Traits> struct ExecutionReport {
  using NanoTimestamp = Traits::NanoTimestamp;
  using OrderID = Traits::OrderID;
  using Instrument = Traits::Instrument;
  using Side = Traits::Side;
  using Price = Traits::Price;
  using Size = Traits::Size;

  NanoTimestamp time;
  OrderID id;
  Side side;
  Instrument instrument;
  Price fill_px;
  Size mached_size;
};
} // namespace execution
