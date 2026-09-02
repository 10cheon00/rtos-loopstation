#include "track_state_overdubbing.h"

#include <array>

#include "utils.h"

static constexpr auto track_state_overdubbing_transition_table = [] {
  std::array<TrackStateId, TRACK_ACTION_ID_COUNT> values{};
  values[TRACK_ACTION_ID_NONE] = TrackStateId::OVERDUBBING;
  values[TRACK_ACTION_ID_ENTER_RECORD_PLAY] = TrackStateId::PLAYING;
  values[TRACK_ACTION_ID_ENTER_STOP] = TrackStateId::STOPPED;
  return values;
}();

static void TrackStateOverdubbing_OnEnter(TrackStateMachineContext* context);

TrackState TRACK_STATE_OVERDUBBING = {
    .id = TrackStateId::OVERDUBBING,
    .transition_table = track_state_overdubbing_transition_table.data(),
    .OnEnter = TrackStateOverdubbing_OnEnter};

static void TrackStateOverdubbing_OnEnter(TrackStateMachineContext* context) {}
