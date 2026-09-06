#include "page.hpp"

PanelSlotType GetPanelSlotType(PageSlotVariant& page_slot_variant) {
  return std::visit([](const auto& value) { return value.GetType(); },
                    page_slot_variant);
}