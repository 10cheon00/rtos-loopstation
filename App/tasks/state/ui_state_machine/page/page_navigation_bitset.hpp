#ifndef PAGE_NAVIGATION_FLAG_HPP
#define PAGE_NAVIGATION_FLAG_HPP

#include <bitset>
#include <cstddef>

#include "enum_id.hpp"
#include "rtos_enum_value.h"

enum class PageNavigation : EnumId {
  NONE,
  LEFT_ARROW,
  RIGHT_ARROW,
  COUNT,
};

class PageNavigationBitset
    : public std::bitset<static_cast<std::size_t>(PageNavigation::COUNT)> {
  using Base = std::bitset<static_cast<std::size_t>(PageNavigation::COUNT)>;

 public:
  PageNavigationBitset() = default;
  PageNavigationBitset(RtosEnumValue rtos_enum_value) : Base(rtos_enum_value) {}

  PageNavigationBitset& operator|=(const PageNavigation page_navigation) {
    if (page_navigation != PageNavigation::NONE) {
      this->set(static_cast<std::size_t>(page_navigation));
    }

    return *this;
  }

  bool HasPageNavigation(PageNavigation page_navigation) const noexcept {
    return this->test(static_cast<EnumId>(page_navigation));
  }

  RtosEnumValue ToRtosEnumValue() const {
    return static_cast<RtosEnumValue>(this->to_ullong());
  }
};

#endif
