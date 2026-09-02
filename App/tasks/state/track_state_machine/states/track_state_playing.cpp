#include "track_state_playing.h"

#include <array>

#include "utils.h"

static constexpr auto track_state_playing_transition_table = [] {
  std::array<TrackStateId, TRACK_ACTION_ID_COUNT> values{};
  values[TRACK_ACTION_ID_NONE] = TrackStateId::PLAYING;
  values[TRACK_ACTION_ID_ENTER_RECORD_PLAY] = TrackStateId::OVERDUBBING;
  values[TRACK_ACTION_ID_ENTER_STOP] = TrackStateId::STOPPED;
  return values;
}();

static void TrackStatePlaying_OnEnter(TrackStateMachineContext* context);

TrackState TRACK_STATE_PLAYING = {
    .id = TrackStateId::PLAYING,
    .transition_table = track_state_playing_transition_table.data(),
    .OnEnter = TrackStatePlaying_OnEnter};

static void TrackStatePlaying_OnEnter(TrackStateMachineContext* context) {}
