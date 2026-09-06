#include "page.hpp"

PageSlotType GetPageSlotType(PageSlotVariant& page_slot_variant) {
  return std::visit([](const auto& value) { return value.GetType(); },
                    page_slot_variant);
}