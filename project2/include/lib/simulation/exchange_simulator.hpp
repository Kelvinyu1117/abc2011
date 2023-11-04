#pragma once

namespace simulation {
class ExchangeSimulator {

public:
  // should have some time priority queue, synchronize market data event and
  // order event, should be multithreaded/parallel, but for now, the logic is it
  // will queue the market data, and trigger the matching based on order: if the
  // latest tick happen before the order, process the tick, and insert the
  // order, if the tick happen after the order, try to match the tick with the
  // order, if it can be matched. update the book and trigger execution,
  // otherwise, process the new order/market data (order) to the orderbook by
  // time priority
};
} // namespace simulation
