#ifndef UI_STATE_SYSTEM_SETTING_PANEL_HPP
#define UI_STATE_SYSTEM_SETTING_PANEL_HPP

#include "ui_state.hpp"

namespace UiStateMachine {

template <typename... Pages>
class SystemSettingState : public FixedPageState<sizeof...(Pages)> {
 public:
  SystemSettingState(Pages... pages)
      : FixedPageState<sizeof...(Pages)>(Id::SYSTEM_SETTING, pages...) {}
};

}  // namespace UiStateMachine

#endif
