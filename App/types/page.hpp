#ifndef PAGE_HPP
#define PAGE_HPP

#include <array>
#include <variant>

#include "menu_descriptor.hpp"
#include "page_slot.hpp"
#include "parameter_descriptor.hpp"
#include "slot_position.hpp"

using PageSlotVariant = std::variant<PageSlot, MenuSlot, ParameterSlot>;

PageSlotType GetPageSlotType(PageSlotVariant& page_slot_variant);

class Page {
 public:
  template <typename... PageSlots>
  constexpr explicit Page(PageSlots... page_slots)
      : page_slots{page_slots...} {}

  PageSlotVariant& GetAt(SlotPosition position) {
    return this->page_slots[static_cast<std::size_t>(position)];
  }

 private:
  std::array<PageSlotVariant, static_cast<std::size_t>(SlotPosition::COUNT)>
      page_slots;
};

#endif
