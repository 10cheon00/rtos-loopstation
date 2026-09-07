#ifndef PAGE_NAVIGATION_FLAG_HPP
#define PAGE_NAVIGATION_FLAG_HPP

#include "enum_id.hpp"

enum class PageNavigationFlag : EnumId {
  NONE = 0x0,
  LEFT_ARROW = 0x1,
  RIGHT_ARROW = 0x2,
};

constexpr PageNavigationFlag operator|(PageNavigationFlag lhs,
                                       PageNavigationFlag rhs) noexcept {
  return static_cast<PageNavigationFlag>(static_cast<EnumId>(lhs) |
                                         static_cast<EnumId>(rhs));
}

constexpr bool HasPageNavigationFlag(PageNavigationFlag flags,
                                     PageNavigationFlag flag) noexcept {
  return (static_cast<EnumId>(flags) & static_cast<EnumId>(flag)) ==
         static_cast<EnumId>(flag);
}

#endif
