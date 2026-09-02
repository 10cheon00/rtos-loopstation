#include "track_state_playing.h"

#include <array>

#include "utils.h"

static constexpr auto track_state_playing_transition_table = [] {
  std::array<TrackStateId, TRACK_ACTION_ID_COUNT> values{};
  values[TRACK_ACTION_ID_NONE] = TRACK_STATE_ID_PLAYING;
  values[TRACK_ACTION_ID_ENTER_RECORD_PLAY] = TRACK_STATE_ID_OVERDUBBING;
  values[TRACK_ACTION_ID_ENTER_STOP] = TRACK_STATE_ID_STOPPED;
  return values;
}();

static void TrackStatePlaying_OnEnter(TrackStateMachineContext* context);

TrackState TRACK_STATE_PLAYING = {
    .id = TRACK_STATE_ID_PLAYING,
    .transition_table = track_state_playing_transition_table.data(),
    .OnEnter = TrackStatePlaying_OnEnter};

static void TrackStatePlaying_OnEnter(TrackStateMachineContext* context) {}
