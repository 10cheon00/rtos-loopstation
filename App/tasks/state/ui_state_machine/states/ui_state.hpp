#ifndef UI_STATE_HPP
#define UI_STATE_HPP

#include <array>
#include <cstdbool>
#include <cstdint>

#include "enum_map.hpp"
#include "page.hpp"
#include "page_group.hpp"
#include "ui_state_id.hpp"

namespace UiStateMachine {

#define PANEL_LABEL_LENGTH 16
using LabelCharArray = std::array<char, PANEL_LABEL_LENGTH>;

constexpr LabelCharArray MakePanelLabel(const char* text) {
  LabelCharArray label{};
  if (text == nullptr) {
    return label;
  }

  for (std::size_t i = 0; i + 1 < label.size() && text[i] != '\0'; ++i) {
    label[i] = text[i];
  }
  return label;
}

struct Panel {
  PageGroup* page_group{};
  LabelCharArray label{};
};

class State {
 public:
  State(Id id, Panel panel) : id(id), page_index(0), panel(panel) {}

  const Id GetId() { return this->id; }

  const std::size_t GetPageIndex() const { return page_index; }

  void ResetPageIndex() { page_index = 0; }

  void IncreasePageIndex() {
    if (CanIncreasePageIndex()) {
      page_index++;
    }
  }

  void DecreasePageIndex() {
    if (CanDecreasePageIndex()) {
      page_index--;
    }
  }

  bool CanIncreasePageIndex() { return page_index + 1 < GetPageGroupCount(); }

  bool CanDecreasePageIndex() { return page_index > 0; }

  std::size_t GetPageGroupCount() { return GetPanel().page_group->GetCount(); }

 private:
  const Id id;
  std::size_t page_index;

 public:
  virtual Panel& GetPanel() { return panel; }

  void UpdatePanel(PageGroup& page_group, LabelCharArray label) {
    Panel& panel = GetPanel();
    if (panel.page_group == &page_group && panel.label == label) {
      return;
    }
    panel = {
        .page_group = &page_group,
        .label = label,
    };
    ResetPageIndex();
  }

  Page& GetCurrentPage() { return GetPanel().page_group->GetAt(page_index); }

  LabelCharArray& GetLabel() { return GetPanel().label; }

 private:
  Panel panel;
};

class FixedPanelState : public State {
 public:
  FixedPanelState(Id id, PageGroup& page_group, const char* label)
      : State(id, Panel{&page_group, MakePanelLabel(label)}) {}

  void UpdatePanel(PageGroup& page_group, LabelCharArray label) {
    // NOP;
  }
};

class DynamicPanelState : public State {
 public:
  DynamicPanelState(Id id, PageGroup& page_group, const char* label)
      : State(id, Panel{&page_group, MakePanelLabel(label)}) {}
};

class NoneState : public FixedPanelState {
 public:
  NoneState(PageGroup& page_group)
      : FixedPanelState(UiStateMachine::Id::NONE, page_group, "NONE") {}
};

class HomeState : public FixedPanelState {
 public:
  HomeState(PageGroup& page_group)
      : FixedPanelState(UiStateMachine::Id::HOME, page_group, "HOME") {}
};

class SettingState : public FixedPanelState {
 public:
  SettingState(PageGroup& page_group)
      : FixedPanelState(UiStateMachine::Id::SETTING, page_group, "SETTING") {}
};

class SystemSettingState : public FixedPanelState {
 public:
  SystemSettingState(PageGroup& page_group)
      : FixedPanelState(UiStateMachine::Id::SYSTEM_SETTING, page_group,
                        "SYSTEM SETTING") {}
};

class FxEditState : public DynamicPanelState {
 public:
  FxEditState(PageGroup& page_group)
      : DynamicPanelState(UiStateMachine::Id::FX_EDIT, page_group,
                          "?Fx ? EDIT") {}
};

}  // namespace UiStateMachine

#endif
