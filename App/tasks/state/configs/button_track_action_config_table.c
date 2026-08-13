#include "button_track_action_config_table.h"

#include "config_table.h"
#include "config_validator.h"

#define ENTRIES                                                                                    \
    ConfigTable_1D_ENTRY(BUTTON_ID_TRACK_1_PLAY_RECORD, TRACK_ACTION_ID_ENTER_RECORD_PLAY),        \
        ConfigTable_1D_ENTRY(BUTTON_ID_TRACK_1_STOP, TRACK_ACTION_ID_ENTER_STOP),

ConfigTable_1D_DECLARE_TABLE(ButtonId, TrackActionId, BUTTON_ID_COUNT, ENTRIES);
#undef ENTRIES

ConfigValidator_REGISTER_CONFIG_TABLE_1D(ButtonId, TrackActionId, BUTTON_ID_COUNT,
                                         TRACK_ACTION_ID_NONE, TRACK_ACTION_ID_COUNT,
                                         CONFIG_TABLE_TYPE_NO_NULL_VALUE);

TrackActionId ButtonTrackActionConfigMap_Get(ButtonId id)
{
    if (id <= BUTTON_ID_NONE || id >= BUTTON_ID_COUNT) {
        return TRACK_ACTION_ID_NONE;
    }
    return ConfigTable_1D_GET(ButtonId, TrackActionId, id);
}
