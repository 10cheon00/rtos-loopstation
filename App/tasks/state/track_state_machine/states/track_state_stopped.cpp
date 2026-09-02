#include "track_state_stopped.h"

#include <array>

#include "utils.h"

static constexpr auto track_state_stopped_transition_table = [] {
  std::array<TrackStateId, TRACK_ACTION_ID_COUNT> values{};
  values[TRACK_ACTION_ID_NONE] = TrackStateId::STOPPED;
  values[TRACK_ACTION_ID_ENTER_RECORD_PLAY] = TrackStateId::PLAYING;
  values[TRACK_ACTION_ID_ENTER_STOP] = TrackStateId::STOPPED;
  return values;
}();

static void TrackStateStopped_OnEnter(TrackStateMachineContext* context);

TrackState TRACK_STATE_STOPPED = {
    .id = TrackStateId::STOPPED,
    .transition_table = track_state_stopped_transition_table.data(),
    .OnEnter = TrackStateStopped_OnEnter};

static void TrackStateStopped_OnEnter(TrackStateMachineContext* context) {}
