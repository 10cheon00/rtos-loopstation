#ifndef UI_STATE_H
#define UI_STATE_H

#include <cstdbool>
#include <cstdint>

#include "enum_map.hpp"
#include "menu_descriptor.hpp"
#include "panel_slot_type.h"
#include "parameter_descriptor.h"
#include "ui_state_id.h"
#include "ui_state_slot_index.h"

namespace UiStateMachine {

struct PanelSlot {
  PanelSlotType type;
  union Data {
    MenuDescriptor menu;
    ParameterDescriptor parameter;
  } data;
};

using Page = std::array<PanelSlot, static_cast<std::size_t>(4)>;

class State {
 public:
  State(Id id) : id(id), page_index(0) {}

  const Id GetId() { return this->id; }
  std::size_t GetPageIndex() const { return this->page_index; }

  void IncreasePageIndex() {
    if (this->CanIncreasePageIndex()) {
      page_index++;
    }
  }
  void DecreasePageIndex() {
    if (this->CanDecreasePageIndex()) {
      page_index--;
    }
  }
  bool CanIncreasePageIndex() {
    return this->page_index + 1 < this->GetPageCount();
  }
  bool CanDecreasePageIndex() { return this->page_index > 0; }

  virtual std::size_t GetPageCount() = 0;
  virtual const Page& GetCurrentPage() const = 0;

 private:
  const Id id;
  std::size_t page_index;
};

template <std::size_t PageCount>
class FixedPageState : public State {
 protected:
  template <typename... Pages>
  explicit FixedPageState(Id id, Pages... pages) : State(id), pages{pages...} {}

 public:
  const Page& GetPage() { return this->pages[this->GetPageIndex()]; }

  std::size_t GetPageCount() override { return pages.size(); }
  const Page& GetCurrentPage() const override {
    return this->pages[this->GetPageIndex()];
  };

 private:
  std::array<Page, PageCount> pages;
};

}  // namespace UiStateMachine

#endif
