#ifndef UI_STATE_HOME_PANEL_H
#define UI_STATE_HOME_PANEL_H

#include "ui_state.h"

namespace UiStateMachine {

template <typename... Pages>
class HomeState : public FixedPageState<sizeof...(Pages)> {
 public:
  HomeState(Pages... pages) : FixedPageState<sizeof...(Pages)>(Id::HOME, pages...) {}
};

}  // namespace UiStateMachine
#endif