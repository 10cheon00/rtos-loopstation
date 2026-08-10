#include "button_track_action_config_map.h"

#include "config_map.h"
#include "config_validator.h"

static ConfigMapEntry button_track_action_entries[] = {
    {.key = BUTTON_ID_TRACK_1_PLAY_RECORD, .value = TRACK_ACTION_ID_ENTER_RECORD_PLAY},
    {.key = BUTTON_ID_TRACK_1_STOP, .value = TRACK_ACTION_ID_ENTER_STOP},
};

static ConfigMap button_track_action_map = {.entries = button_track_action_entries,
                                            .count = ARRAY_COUNT(button_track_action_entries)};
// 생성한 map을 설정 검증자에 등록한다.
ConfigValidator_REGISTER(&button_track_action_map, ButtonId, TrackActionId);

TrackActionId ButtonTrackActionConfigMap_Get(ButtonId id)
{
    TrackActionId track_action_id;
    if (ConfigMap_Get(&button_track_action_map, id, (Value_t *)&track_action_id) ==
        CONFIG_MAP_RESULT_OK) {
        return track_action_id;
    }
    return TRACK_ACTION_ID_NONE;
}
