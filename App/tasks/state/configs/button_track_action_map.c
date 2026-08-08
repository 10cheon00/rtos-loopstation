#include "button_track_action_map.h"

#include "utils.h"

static ButtonTrackActionMapEntry button_track_action_map[] = {
    {.button_id = BUTTON_ID_TRACK_1_PLAY_RECORD, .track_action_id = TRACK_ACTION_ID_ENTER_RECORD_PLAY},
    {.button_id = BUTTON_ID_TRACK_1_STOP, .track_action_id = TRACK_ACTION_ID_ENTER_STOP},
};

static size_t button_track_action_map_count = ARRAY_COUNT(button_track_action_map);

TrackActionId ButtonTrackActionMap_GetTrackActionId(ButtonId id)
{
    for (size_t i=0; i<button_track_action_map_count;i++) {
        if (button_track_action_map[i].button_id == id) {
            return button_track_action_map[i].track_action_id;
        }
    }
    return TRACK_ACTION_ID_NONE;
}
