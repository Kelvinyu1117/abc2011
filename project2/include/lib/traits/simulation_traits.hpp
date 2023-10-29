#include "lib/symbology/instrument.hpp"
#include <cstdint>
namespace traits {
struct SimulationTraits {
  using Price = double; // use double for now, maybe should use integer type
  using Size = uint64_t;
  using Instrument = symbology::Instrument;

  //   struct MDTraits {
  //     using Price = double; // use double for now, some may use integer type
  //     using Size = uint64_t;
  //     using Instrument = symbology::Instrument;
  //   };

  //   struct ExecutionTraits {};
};

} // namespace traits
