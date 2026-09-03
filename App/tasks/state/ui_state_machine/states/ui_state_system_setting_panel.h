#ifndef UI_STATE_SYSTEM_SETTING_PANEL_H
#define UI_STATE_SYSTEM_SETTING_PANEL_H

#include "ui_state.h"

namespace UiStateMachine {

template <typename... Pages>
class SystemSettingState : public FixedPageState<sizeof...(Pages)> {
 public:
  SystemSettingState(Pages... pages)
      : FixedPageState<sizeof...(Pages)>(Id::SYSTEM_SETTING, pages...) {}
};

}  // namespace UiStateMachine

#endif
