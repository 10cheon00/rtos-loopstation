#include "page.hpp"

PageSlotType GetPageSlotType(PageSlotVariant& page_slot_variant) {
  if (std::holds_alternative<MenuSlot>(page_slot_variant)) {
    return PageSlotType::MENU;
  }

  if (std::holds_alternative<ParameterSlot>(page_slot_variant)) {
    return PageSlotType::PARAMETER;
  }
  return PageSlotType::NONE;
}