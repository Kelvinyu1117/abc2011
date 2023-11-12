#pragma once
#include "lib/execution/execution_event_listener.hpp"
#include "lib/execution/execution_report.hpp"
#include "lib/market_data/md_event_listener.hpp"
#include "lib/simulation/latency_model.hpp"
#include "lib/utils/helpers.hpp"
#include "lib/utils/orderbook.hpp"
#include <atomic>
#include <bits/ranges_algo.h>
#include <functional>
#include <memory>
#include <queue>
#include <utility>
#include <variant>
namespace simulation {

template <typename Traits> struct NewOrderEvent {
  using OrderInfo = utils::Order<Traits>;
  using NanoTimestamp = Traits::NanoTimestamp;

  NanoTimestamp time;
  OrderInfo order_info;
};

template <typename Traits> struct AmendOrderEvent {
  using OrderInfo = utils::Order<Traits>;
  using OrderID = Traits::OrderID;
  using NanoTimestamp = Traits::NanoTimestamp;

  NanoTimestamp time;
  OrderInfo order_info;
};

template <typename Traits> struct CacnelOrderEvent {
  using OrderInfo = utils::Order<Traits>;
  using OrderID = Traits::OrderID;
  using NanoTimestamp = Traits::NanoTimestamp;

  NanoTimestamp time;
  OrderInfo order_info;
};

template <typename Traits>
using OrderEvent = std::variant<NewOrderEvent<Traits>, AmendOrderEvent<Traits>,
                                CacnelOrderEvent<Traits>>;

template <typename Traits>
inline bool operator<(const OrderEvent<Traits> &a,
                      const OrderEvent<Traits> &b) {

  return std::visit(
      [&b](auto &a1) {
        return std::visit([&a1](auto &b1) { return a1.time < b1.time; }, b);
      },
      a);
}

/**
 * @brief It is really for demo the idea for exchange simulator
 *
 * @tparam Traits
 */
template <typename Traits>
class ExchangeSimulator : public market_data::IL3EventListener<Traits> {

public:
  using IExecutionEventListener = execution::IExecutionEventListener<Traits>;
  using Trade = market_data::Trade<Traits>;
  using Side = Traits::Side;
  using Size = Traits::Size;
  using OrderID = Traits::OrderID;
  using NanoTimestamp = Traits::NanoTimestamp;
  using OrderInfo = utils::Order<Traits>;
  using OrderBook = utils::OrderBook<Traits>;
  using LatencyModel = LatencyModel<Traits>;
  using NewOrderEvent = NewOrderEvent<Traits>;
  using AmendOrderEvent = AmendOrderEvent<Traits>;
  using CacnelOrderEvent = CacnelOrderEvent<Traits>;
  using Event = OrderEvent<Traits>;

  using Clock =
      std::atomic_uint64_t; // using a atomic variable to simulate the exchange
                            // clock, it should be monotonically increasing
  using PriorityEventQueue =
      std::priority_queue<Event, std::vector<Event>, std::less<Event>>;

  using ExecutionInfo = execution::ExecutionReport<Traits>;

  ExchangeSimulator(std::shared_ptr<LatencyModel> latency_model,
                    IExecutionEventListener &execution_listener)
      : latency_model{latency_model}, book{std::make_unique<OrderBook>()},
        execution_listener{execution_listener} {}
  // should have some time priority queue to queue the order by time
  // priority, and trigger the matching

  void process_event() // this is only use in the demo and it will be triggered
                       // by send_order from the executionContext (not consider
                       // thread-safety), in practice it should be a busy loop
                       // in a independent thread keep polling the internal
                       // event queue (lock free)
  {
    while (!event_queue.empty()) {
      auto event = event_queue.top();
      event_queue.pop();

      std::visit(
          utils::overloaded{[this](NewOrderEvent &event) {
                              execution_listener.on_send_order(
                                  *book, event.order_info);
                              if (!try_match(event.time, event.order_info)) {
                                book->insert(event.order_info);
                              }
                            },
                            [this](AmendOrderEvent &event) {
                              try_amend(event.time, event.order_info);
                            },
                            [this](CacnelOrderEvent &event) {
                              try_cancel(event.time, event.order_info);
                            }},
          event);
    }
  }

  void on_trade(const Trade &trade) override {}

  void on_order_add(const OrderInfo &order) override {
    on_receive_new_order(order);
  }

  void on_order_cancel(const OrderInfo &order) override {
    on_receive_cancel_order(order);
  }

  void on_receive_new_order(const OrderInfo &order) {
    event_queue.push(Event{NewOrderEvent{
        order.time + latency_model->get_order_entry_latency(), order}});
  }

  void on_receive_amend_order(const OrderInfo &order) {
    event_queue.push(Event{AmendOrderEvent{
        order.time + latency_model->get_order_entry_latency(), order}});
  }

  void on_receive_cancel_order(const OrderInfo &order) {
    event_queue.push(Event{CacnelOrderEvent{
        order.time + latency_model->get_order_entry_latency(), order}});
  }

private:
  NanoTimestamp get_current_time(NanoTimestamp ref_time = 0) {
    return ref_time + clock.fetch_add(1, std::memory_order_relaxed);
  }

  void on_execution(OrderBook &orderbook, const OrderInfo &order_info,
                    ExecutionInfo report) {
    execution_listener.on_order_execution(orderbook, order_info, report);
  }

  bool try_match(NanoTimestamp event_time, OrderInfo &order_info) {
    const auto order = order_info;

    if (order_info.side == Side::BID) {
      if (book->template getNumOfLevels<Side::ASK>() == 0) {
        return false;
      }

      auto it = book->template begin<Side::ASK>();
      auto px = order_info.pxsz.px;
      auto amt = order_info.pxsz.sz;

      if (px < it->first) {
        return false;
      }

      bool is_done = false;

      while (!is_done && px >= it->first &&
             it != book->template end<Side::ASK>()) {
        auto &orderQueue = it->second;

        bool isLevelCleared = false;
        while (!orderQueue.empty() && !isLevelCleared) {
          auto &front_order = orderQueue.front();
          auto front_order_id = front_order.id;
          auto front_order_px = front_order.pxsz.px;
          auto front_order_size = front_order.pxsz.sz;
          auto matched_sz = std::min(front_order_size, amt);

          auto fill_px = std::min(front_order_px, order_info.pxsz.px);

          front_order.pxsz.sz = (front_order_size - matched_sz);
          if (front_order.pxsz.sz == 0) {
            orderQueue.pop();
          }

          on_execution(
              *book, order,
              ExecutionInfo{get_current_time(event_time) +
                                latency_model->get_order_response_latency(),
                            order.id, Side::BID, order.instrument, fill_px,
                            matched_sz});

          amt = std::max(static_cast<Size>(0), amt - matched_sz);
          order_info.pxsz.sz = amt;

          isLevelCleared = amt == 0;
        }

        is_done = order_info.pxsz.sz == 0;
        if (orderQueue.empty()) {
          book->template erase<Side::ASK>(it++);
        }

        return is_done;
      }
    } else if (order_info.side == Side::ASK) {
      if (book->template getNumOfLevels<Side::BID>() == 0) {
        return false;
      }

      auto it = book->template begin<Side::BID>();
      auto px = order_info.pxsz.px;
      auto amt = order_info.pxsz.sz;

      if (px > it->first) {
        return false;
      }

      bool is_done = false;

      while (!is_done && px <= it->first &&
             it != book->template end<Side::BID>()) {
        auto &orderQueue = it->second;

        bool isLevelCleared = false;
        while (!orderQueue.empty() && !isLevelCleared) {
          auto &front_order = orderQueue.front();
          auto front_order_id = front_order.id;
          auto front_order_px = front_order.pxsz.px;
          auto front_order_size = front_order.pxsz.sz;
          auto matched_sz = std::min(front_order_size, amt);
          auto fill_px = std::min(front_order_px, order_info.pxsz.px);

          front_order.pxsz.sz = front_order_size - matched_sz;
          if (front_order.pxsz.sz == 0) {
            orderQueue.pop();
          }

          on_execution(
              *book, order,
              ExecutionInfo{get_current_time(event_time) +
                                latency_model->get_order_response_latency(),
                            order.id, Side::ASK, order.instrument, fill_px,
                            matched_sz});

          amt = std::max(static_cast<Size>(0), amt - matched_sz);
          order_info.pxsz.sz = amt;
          isLevelCleared = amt == 0;
        }

        is_done = order_info.pxsz.sz == 0;

        if (orderQueue.empty()) {
          book->template erase<Side::BID>(it++);
        }
      }

      return is_done;
    }

    return false;
  }

  bool try_amend(NanoTimestamp event_time, OrderInfo &order_info) {
    // find the order, and remove from the queue, update the order and insert
    // back the queue (lose the priority)
    if (try_cancel(event_time, order_info)) {
      if (!try_match(event_time, order_info)) {
        book->insert(order_info);
      }
    }
    return false;
  }

  bool try_cancel(NanoTimestamp event_time, OrderInfo &order_info) {
    // find and remove the order from the queue, assume the order must be in, in
    // reality, it should throw out a cancel reject to the client side.

    auto impl = [this]<Side side>(OrderInfo &order_info) {
      if (auto it = book->template search<side>(order_info.pxsz.px);
          it != book->template end<side>()) {

        auto &queue = it->second.internal();

        if (auto target_order =
                std::ranges::find_if(queue,
                                     [&order_info](const auto &order) {
                                       return order.id == order_info.id;
                                     });
            target_order != queue.end()) {
          queue.erase(target_order);
          return true;
        }
      }

      return false;
    };

    if (order_info.side == Side::BID) {
      return impl.template operator()<Side::BID>(order_info);
    }

    else if (order_info.side == Side::ASK) {
      return impl.template operator()<Side::ASK>(order_info);
    }

    return false;
  }

private:
  std::unique_ptr<OrderBook> book;
  std::shared_ptr<LatencyModel> latency_model;
  IExecutionEventListener &execution_listener;
  PriorityEventQueue event_queue; // event Queue based on time priority
  Clock clock;
};

} // namespace simulation
