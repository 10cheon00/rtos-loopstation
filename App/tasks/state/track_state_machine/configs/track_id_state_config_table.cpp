#include "track_id_state_config_table.h"

#include "config_table.h"
#include "config_validator.h"
#include "track_state_idle.h"
#include "track_state_overdubbing.h"
#include "track_state_playing.h"
#include "track_state_recording.h"
#include "track_state_stopped.h"

#define ENTRIES                                                               \
  ConfigTable_1D_ENTRY(TRACK_STATE_ID_NONE, nullptr),                         \
      ConfigTable_1D_ENTRY(TRACK_STATE_ID_IDLE, &TRACK_STATE_IDLE),           \
      ConfigTable_1D_ENTRY(TRACK_STATE_ID_RECORDING, &TRACK_STATE_RECORDING), \
      ConfigTable_1D_ENTRY(TRACK_STATE_ID_STOPPED, &TRACK_STATE_STOPPED),     \
      ConfigTable_1D_ENTRY(TRACK_STATE_ID_PLAYING, &TRACK_STATE_PLAYING),     \
      ConfigTable_1D_ENTRY(TRACK_STATE_ID_OVERDUBBING,                        \
                           &TRACK_STATE_OVERDUBBING)

ConfigTable_1D_DECLARE_TABLE(TrackStateId, TrackStatePointer,
                             TRACK_STATE_ID_COUNT, ENTRIES);
#undef ENTRIES

ConfigValidator_REGISTER_CONFIG_TABLE_1D(TrackStateId, TrackStatePointer,
                                         TRACK_STATE_ID_COUNT, 0, UINT32_MAX,
                                         CONFIG_TABLE_TYPE_NO_NULL_VALUE);

TrackStatePointer TrackIdStateConfigMap_Get(TrackStateId id) {
  if (id <= TRACK_STATE_ID_NONE || id >= TRACK_STATE_ID_COUNT) {
    return NULL;
  }
  return ConfigTable_1D_GET(TrackStateId, TrackStatePointer, id);
}
