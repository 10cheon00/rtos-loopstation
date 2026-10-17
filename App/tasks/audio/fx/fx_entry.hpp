#ifndef FX_ENTRY_HPP
#define FX_ENTRY_HPP

#include <array>
#include <cstdbool>
#include <cstddef>

#include "fx_type.hpp"
#include "fx_slot.hpp"
#include "fx_variant.hpp"

namespace Fx {

struct Entry {
  Fx::Type type;
  Fx::Variant fx;
  bool is_on;
};

using SlotEntries = std::array<Entry, static_cast<std::size_t>(Slot::COUNT)>;

}  // namespace Fx

#endif
