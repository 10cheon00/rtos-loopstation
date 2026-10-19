#ifndef UI_STATE_HPP
#define UI_STATE_HPP

#include <cstdbool>
#include <cstdint>

#include "enum_map.hpp"
#include "page.hpp"
#include "page_group.hpp"
#include "ui_state_id.hpp"

namespace UiStateMachine {

class State {
 public:
  State(Id id, const char* label, PageGroup& page_group)
      : id(id), label(label), page_group(page_group), page_index(0) {}

  const Id GetId() { return this->id; }

  const std::size_t GetPageIndex() const { return this->page_index; }

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
    return this->page_index + 1 < this->GetPageGroupCount();
  }

  bool CanDecreasePageIndex() { return this->page_index > 0; }

  std::size_t GetPageGroupCount() { return this->page_group.GetCount(); }

  Page& GetCurrentPage() { return this->page_group.GetAt(this->page_index); }

 private:
  const Id id;
  const char* label;
  PageGroup& page_group;
  std::size_t page_index;
};

class NoneState : public UiStateMachine::State {
 public:
  NoneState(PageGroup& page_group)
      : State(UiStateMachine::Id::NONE, "NONE", page_group) {}
};

class HomeState : public UiStateMachine::State {
 public:
  HomeState(PageGroup& page_group)
      : State(UiStateMachine::Id::HOME, "HOME", page_group) {}
};

class SettingState : public UiStateMachine::State {
 public:
  SettingState(PageGroup& page_group)
      : State(UiStateMachine::Id::SETTING, "SETTING", page_group) {}
};

class SystemSettingState : public UiStateMachine::State {
 public:
  SystemSettingState(PageGroup& page_group)
      : State(UiStateMachine::Id::SYSTEM_SETTING, "SYSTEM SETTING",
              page_group) {}
};

class FxEditState : public UiStateMachine::State {
 public:
  FxEditState(PageGroup& page_group)
      : State(UiStateMachine::Id::FX_EDIT, "FX EDIT", page_group) {}
};

}  // namespace UiStateMachine

#endif
