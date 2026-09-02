#include "track_state_idle.h"

#include <array>

#include "utils.h"

static constexpr auto track_state_idle_transition_table = [] {
  std::array<TrackStateId, TRACK_ACTION_ID_COUNT> values{};
  values[TRACK_ACTION_ID_NONE] = TRACK_STATE_ID_IDLE;
  values[TRACK_ACTION_ID_ENTER_RECORD_PLAY] = TRACK_STATE_ID_RECORDING;
  values[TRACK_ACTION_ID_ENTER_STOP] = TRACK_STATE_ID_IDLE;
  return values;
}();

static void TrackStateIdle_OnEnter(TrackStateMachineContext* context);

TrackState TRACK_STATE_IDLE = {
    .id = TRACK_STATE_ID_IDLE,
    .transition_table = track_state_idle_transition_table.data(),
    .OnEnter = TrackStateIdle_OnEnter};

static void TrackStateIdle_OnEnter(TrackStateMachineContext* context) {}
