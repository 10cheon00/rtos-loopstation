#ifndef PAGE_HPP
#define PAGE_HPP

#include <array>
#include <variant>

#include "menu_descriptor.hpp"
#include "page_slot.hpp"
#include "slot_index.hpp"

using PageSlotVariant = std::variant<PageSlot, MenuSlot, ParameterSlot>;

PageSlotType GetPageSlotType(PageSlotVariant& page_slot_variant);

class Page {
 public:
  template <typename... PageSlots>
  constexpr explicit Page(PageSlots... page_slots)
      : page_slots{page_slots...} {}

  PageSlotVariant& GetAt(SlotIndex index) {
    if (index >= SlotIndex::INVALID) {
      return this->dummy_page_slot;
    }
    return this->page_slots[static_cast<std::size_t>(index)];
  }

  template <typename PageSlotType>
  bool IsTypeAt(SlotIndex index) {
    return std::holds_alternative<PageSlotType>(this->GetAt(index));
  }

 private:
  std::array<PageSlotVariant, static_cast<std::size_t>(SlotIndex::COUNT)>
      page_slots;
  PageSlotVariant dummy_page_slot{PageSlot{}};
};

#endif
