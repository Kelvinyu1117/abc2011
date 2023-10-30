#pragma once

#include "lib/traits/data_traits.hpp"
#include "lib/utils/order.hpp"
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
  using Price = DataTraits::IntPrice;

  using BidMap = DataTraits::template Map<Price, OrderQueue, std::greater<>>;
  using AskMap = DataTraits::template Map<Price, OrderQueue>;
  using Order = utils::Order<DataTraits>;

public:
  using BidSideIterator = BidMap::iterator;
  using AskSideIterator = AskMap::iterator;

  void insert(const Order &order) {
    if (order.side == Side::BID) {
      bids[order.getPrice()].push(order);
    } else {
      asks[order.getPrice()].push(order);
    }
  }

  void insert(const Order &&order) {
    if (order.side == Side::BID) {
      bids[order.mPrice].push(std::move(order));
    } else {
      asks[order.mPrice].push(std::move(order));
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
  BidMap bids;
  AskMap asks;
};
} // namespace utils
