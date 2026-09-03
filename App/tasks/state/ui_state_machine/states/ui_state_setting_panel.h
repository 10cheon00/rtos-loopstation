#ifndef UI_STATE_SETTING_PANEL_H
#define UI_STATE_SETTING_PANEL_H

#include "ui_state.h"

namespace UiStateMachine {

template <typename... Pages>
class SettingState : public FixedPageState<sizeof...(Pages)> {
 public:
  SettingState(Pages... pages)
      : FixedPageState<sizeof...(Pages)>(Id::SETTING, pages...) {}
};

}  // namespace UiStateMachine

#endif