#ifndef UI_STATE_SETTING_PANEL_HPP
#define UI_STATE_SETTING_PANEL_HPP

#include "ui_state.hpp"

namespace UiStateMachine {

template <typename... Pages>
class SettingState : public FixedPageState<sizeof...(Pages)> {
 public:
  SettingState(Pages... pages)
      : FixedPageState<sizeof...(Pages)>(Id::SETTING, pages...) {}
};

}  // namespace UiStateMachine

#endif
