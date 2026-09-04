#ifndef PAGE_HPP
#define PAGE_HPP

#include <array>

#include "menu_descriptor.hpp"
#include "panel_slot_type.h"
#include "parameter_descriptor.hpp"
#include "slot_position.hpp"

struct PanelSlot {
  PanelSlotType type;
  union Data {
    MenuDescriptor menu;
    ParameterDescriptor parameter;
  } data;
};

class Page {
 public:
  template <typename... PanelSlots>
  constexpr explicit Page(PanelSlots... panel_slots) : panel_slots{panel_slots...}{
  }
  const PanelSlot& GetPanelSlotAt(SlotPosition position) const {
    return this->panel_slots[static_cast<std::size_t>(position)];
  }

  const PanelSlot& operator[](SlotPosition position) const {
    return this->panel_slots[static_cast<std::size_t>(position)];
  }

 private:
  std::array<PanelSlot, static_cast<std::size_t>(SlotPosition::COUNT)>
      panel_slots;
};

#endif
