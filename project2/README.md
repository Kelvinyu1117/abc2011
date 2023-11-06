# Project 2 - Trade Simulator

In this project, a trading simulator framework is designed for backtesting strategies. Although it is not a compilable and production-ready system at the moment, the skeleton of the design was implemented and it can easily be extended to accommodate live trading such that we can use the same codebase for both live trading and simulation.

## Required Environment

* Linux (Developed on Ubuntu 20.04)
* CMake 3.21
* Clang/Clang++ 16
  
## Build and execution

```bash
cmake -S . -B build
cd build
make
./trade-simulator sim
```

## Design

![design-img](./imgs/design.png "design-img")

The above diagram illustrates the design of the system, there are three main components in this application:

* Market Data Context
* Strategy Container
* Execution Context

The green arrow represents the live data flow, and the red arrow represents the simulation data flow.

### Market Data Context

This component represents a market data client to the external system. For example, the `class LiveMDContext` represents the real-time live market data connector from the external market data gateway application (with the assumption that the market data message exchange format is the same), through either TCP, UDP, or shared memory. The `class HistoricalMDContext` represents the historical data connector from the external static data source, like raw data or database. With the assumption of L3 Orderbook data (trades and individual orders), the market data context provides consistent interfaces for different listeners (Strategy, Exchange Simulator,..) to handle the market data:

```cpp
// IL3EventListener
void on_trade(const Trade &trade);

void on_order_add(const OrderInfo &order);

void on_order_cancel(const OrderInfo &order);
```

### Strategy Container

This component (`class Trader`) acts as a middleware that connects the MarketDataContext and ExecutionContext with the strategies (signal generation). The market data (trade, order) will be received by the above market data context's callbacks, and the corresponding strategies, for example, some strategies may work on the individual orders, and some of the strategies may work on the market depth levels. In this demonstration, a simple EMA Strategy operates on `Trade` data and returns the signal (long/short) to the middleware based on the calculation, such that the middleware can forward the signals to the execution context. This component will implement the following interface to forward the signals to the execution context:

```cpp
// Signal
enum class Signal { LONG, SHORT, IDLE };
// ISignalListener
void on_signal(Signal signal);
```

However, the actual execution is not determined by the strategy, the execution modules will decide whether we want to open/close the position (send order) based on various factors, e.g.risk exposure. The signal can be ignored by the execution modules if some conditions are not satisfied.

### Execution Context

This component represents the execution algorithms, position manager, as well as order entry client. In this component, The `class LiveExecutionContext` represents the order entry client that connects to the external order management system, and the `class SimulationExecutionContext` will route the orders to the `class ExchangeSimulator` and the `ExchangeSimulator` will construct an L3 order book and try to do the order matching based on the order received in the strategy and the historical market data. Both `LiveExecutionContext` and `SimulationExecutionContext` process the order response to the strategy through the following callbacks:

```cpp
void on_order_execution(OrderBook &orderbook, const OrderInfo &order_info,
                          const ExecutionInfo &executionInfo);

void on_send_order(OrderBook &order, const OrderInfo &order_info);
``` 

