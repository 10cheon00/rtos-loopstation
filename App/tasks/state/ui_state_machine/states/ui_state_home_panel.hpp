#ifndef UI_STATE_HOME_PANEL_HPP
#define UI_STATE_HOME_PANEL_HPP

#include "ui_state.hpp"

namespace UiStateMachine {

template <typename... Pages>
class HomeState : public FixedPageState<sizeof...(Pages)> {
 public:
  HomeState(Pages... pages)
      : FixedPageState<sizeof...(Pages)>(Id::HOME, pages...) {}
};

}  // namespace UiStateMachine
#endif
