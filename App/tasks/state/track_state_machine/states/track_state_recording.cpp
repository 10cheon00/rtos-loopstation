#include "track_state_recording.h"

#include <array>

#include "utils.h"

static constexpr auto track_state_recording_transition_table = [] {
  std::array<TrackStateId, TRACK_ACTION_ID_COUNT> values{};
  values[TRACK_ACTION_ID_NONE] = TRACK_STATE_ID_RECORDING;
  values[TRACK_ACTION_ID_ENTER_RECORD_PLAY] = TRACK_STATE_ID_PLAYING;
  values[TRACK_ACTION_ID_ENTER_STOP] = TRACK_STATE_ID_STOPPED;
  return values;
}();

static void TrackStateRecording_OnEnter(TrackStateMachineContext* context);

TrackState TRACK_STATE_RECORDING = {
    .id = TRACK_STATE_ID_RECORDING,
    .transition_table = track_state_recording_transition_table.data(),
    .OnEnter = TrackStateRecording_OnEnter};

static void TrackStateRecording_OnEnter(TrackStateMachineContext* context) {}
