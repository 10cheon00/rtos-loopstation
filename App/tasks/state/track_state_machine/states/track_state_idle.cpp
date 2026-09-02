#include "track_state_idle.h"

#include <array>

#include "utils.h"

static constexpr auto track_state_idle_transition_table = [] {
  std::array<TrackStateId, TRACK_ACTION_ID_COUNT> values{};
  values[TRACK_ACTION_ID_NONE] = TrackStateId::IDLE;
  values[TRACK_ACTION_ID_ENTER_RECORD_PLAY] = TrackStateId::RECORDING;
  values[TRACK_ACTION_ID_ENTER_STOP] = TrackStateId::IDLE;
  return values;
}();

static void TrackStateIdle_OnEnter(TrackStateMachineContext* context);

TrackState TRACK_STATE_IDLE = {
    .id = TrackStateId::IDLE,
    .transition_table = track_state_idle_transition_table.data(),
    .OnEnter = TrackStateIdle_OnEnter};

static void TrackStateIdle_OnEnter(TrackStateMachineContext* context) {}
