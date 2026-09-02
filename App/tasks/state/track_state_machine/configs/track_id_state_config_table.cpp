#include "track_id_state_config_table.h"

#include "enum_map.hpp"
#include "track_state_idle.h"
#include "track_state_overdubbing.h"
#include "track_state_playing.h"
#include "track_state_recording.h"
#include "track_state_stopped.h"

static constexpr EnumMap<TrackStateId, TrackStatePointer> track_state_map{
    EnumEntry{TrackStateId::NONE, nullptr},
    EnumEntry{TrackStateId::IDLE, &TRACK_STATE_IDLE},
    EnumEntry{TrackStateId::RECORDING, &TRACK_STATE_RECORDING},
    EnumEntry{TrackStateId::STOPPED, &TRACK_STATE_STOPPED},
    EnumEntry{TrackStateId::PLAYING, &TRACK_STATE_PLAYING},
    EnumEntry{TrackStateId::OVERDUBBING, &TRACK_STATE_OVERDUBBING},
};

TrackStatePointer TrackIdStateConfigMap_Get(TrackStateId id) {
  if (id <= TrackStateId::NONE || id >= TrackStateId::COUNT) {
    return NULL;
  }
  return track_state_map[id];
}
