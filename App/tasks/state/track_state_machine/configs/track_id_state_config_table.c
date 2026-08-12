#include "track_id_state_config_table.h"

#include "track_state_idle.h"
#include "track_state_recording.h"
#include "track_state_stopped.h"
#include "track_state_playing.h"
#include "track_state_overdubbing.h"
#include "config_table.h"

#define ENTRIES                                                                                    \
    ConfigTable_1D_ENTRY(TRACK_STATE_ID_NONE, NULL),                                               \
        ConfigTable_1D_ENTRY(TRACK_STATE_ID_IDLE, &TRACK_STATE_IDLE),                              \
        ConfigTable_1D_ENTRY(TRACK_STATE_ID_RECORDING, &TRACK_STATE_RECORDING),                    \
        ConfigTable_1D_ENTRY(TRACK_STATE_ID_STOPPED, &TRACK_STATE_STOPPED),                        \
        ConfigTable_1D_ENTRY(TRACK_STATE_ID_PLAYING, &TRACK_STATE_PLAYING),                        \
        ConfigTable_1D_ENTRY(TRACK_STATE_ID_OVERDUBBING, &TRACK_STATE_OVERDUBBING),

ConfigTable_1D_DECLARE_TABLE(TrackStateId, TrackStatePointer, TRACK_STATE_ID_COUNT, ENTRIES);
#undef ENTRIES

// ConfigValidator_REGISTER(&track_id_state_config_map, TrackStateId, TrackStatePointer);

TrackStatePointer TrackIdStateConfigMap_Get(TrackStateId id)
{
    if (id <= TRACK_STATE_ID_NONE || id >= TRACK_STATE_ID_COUNT) {
        return NULL;
    }
    return ConfigTable_NAME(TrackStateId, TrackStatePointer)[id];
}
