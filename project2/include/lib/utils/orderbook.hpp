#pragma once

#include "lib/traits/data_traits.hpp"
#include "lib/utils/order.hpp"
#include <cmath>
#include <queue>
namespace utils {

template <typename DataTraits> class OrderQueue {
  using Order = utils::Order<DataTraits>;
  using Queue = DataTraits::template Queue<Order>;

public:
  auto &front() { return queue.front(); }

  void push(Order &&order) { queue.emplace(std::move(order)); }

  void push(const Order &order) { queue.emplace(order); }

  void pop() { return queue.pop(); };

  bool empty() const { return queue.empty(); }

  size_t size() const { return queue.size(); }

private:
  Queue queue;
};

template <typename DataTraits> class OrderBook {
  using Price = DataTraits::Price;
  using IntPrice = DataTraits::IntPrice;

  using BidMap = DataTraits::template Map<Price, OrderQueue, std::greater<>>;
  using AskMap = DataTraits::template Map<Price, OrderQueue>;
  using Order = utils::Order<DataTraits>;

public:
  using BidSideIterator = BidMap::iterator;
  using AskSideIterator = AskMap::iterator;

  void insert(const Order &order) {
    if (order.side == Side::BID) {
      bids[to_int_px(order.pxsz.px)].push(order);
    } else {
      asks[to_int_px(order.pxsz.px)].push(order);
    }
  }

  void insert(const Order &&order) {
    if (order.side == Side::BID) {
      bids[to_int_px(order.pxsz.px)].push(std::move(order));
    } else {
      asks[to_int_px(order.pxsz.px)].push(std::move(order));
    }
  }

  template <Side side> auto begin() {
    if constexpr (side == Side::BID) {
      return bids.begin();
    } else {
      return asks.begin();
    }
  }

  template <Side side> auto end() {
    if constexpr (side == Side::BID) {
      return bids.end();
    } else {
      return asks.end();
    }
  }

  template <Side side> auto search(Price px) {
    if constexpr (side == Side::BID) {
      return bids.lower_bound(px);
    } else {
      return asks.lower_bound(px);
    }
  }

  template <Side side> size_t getNumOfLevels() const {
    if constexpr (side == Side::BID) {
      return bids.size();
    } else {
      return asks.size();
    }
  }
  BidSideIterator erase(const BidSideIterator &it);
  AskSideIterator erase(const AskSideIterator &it);

private:
  // multiply the floating point px by a factor(10^3) to convert it to be a
  // integral
  constexpr IntPrice to_int_px(Price px) { return IntPrice(px * px_multipier); }

  // convert back the IntPrice back to floating point price by divided by the
  // same factor;
  constexpr Price to_px(IntPrice px) { return Price(px / px_multipier); }

private:
  const double px_multipier = std::pow(10, 3);
  BidMap bids;
  AskMap asks;
};
} // namespace utils
