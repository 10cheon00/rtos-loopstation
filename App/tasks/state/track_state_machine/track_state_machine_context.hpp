#ifndef TRACK_STATE_MACHINE_CONTEXT_HPP
#define TRACK_STATE_MACHINE_CONTEXT_HPP

#include "cmsis_os2.h"
#include "track_setting.hpp"

namespace TrackStateMachine {

struct Context {
  TrackSetting track_setting;
};

void InitContext(Context* context);

}  // namespace TrackStateMachine

#endif