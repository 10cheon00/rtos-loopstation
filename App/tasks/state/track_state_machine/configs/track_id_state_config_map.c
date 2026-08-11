#include "track_id_state_config_map.h"

#include "track_state_idle.h"
#include "track_state_recording.h"
#include "track_state_stopped.h"
#include "track_state_playing.h"
#include "track_state_overdubbing.h"
#include "config_map.h"
#include "config_validator.h"

static ConfigMapEntry track_id_state_config_map_entries[] = {
    {.key = TRACK_STATE_ID_NONE, .value = (Value_t)NULL},
    {.key = TRACK_STATE_ID_IDLE, .value = (Value_t)&TRACK_STATE_IDLE},
    {.key = TRACK_STATE_ID_RECORDING, .value = (Value_t)&TRACK_STATE_RECORDING},
    {.key = TRACK_STATE_ID_STOPPED, .value = (Value_t)&TRACK_STATE_STOPPED},
    {.key = TRACK_STATE_ID_PLAYING, .value = (Value_t)&TRACK_STATE_PLAYING},
    {.key = TRACK_STATE_ID_OVERDUBBING, .value = (Value_t)&TRACK_STATE_OVERDUBBING},
};

static ConfigMap track_id_state_config_map = {.entries = track_id_state_config_map_entries,
                                              .count =
                                                  ARRAY_COUNT(track_id_state_config_map_entries)};

ConfigValidator_REGISTER(&track_id_state_config_map, TrackStateId, TrackStatePointer);

TrackStatePointer TrackIdStateConfigMap_Get(TrackStateId id)
{
    Value_t value;
    if (ConfigMap_Get(&track_id_state_config_map, id, &value) != CONFIG_MAP_RESULT_OK) {
        return NULL;
    }
    return (TrackStatePointer)value;
}
