#pragma once

#include "lib/market_data/md_utils.hpp"
namespace utils {

enum class OrderAction { ADD, CANCEL };

template <typename DataTraits> class Order {
  using OrderID = DataTraits::OrderID;
  using Instrument = DataTraits::Instrument;
  using NanoTimestamp = DataTraits::NanoTimestamp;

  OrderID id;
  OrderAction action_;
  Instrument instrument_;
  NanoTimestamp time;
  utils::PriceSize<DataTraits> pxsz;
};
} // namespace utils
