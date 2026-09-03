#include "button_track_action_config_table.h"

#include "button_id.hpp"
#include "enum_map.hpp"

static constexpr EnumMap<ButtonId, TrackActionId> button_track_action_map{
    EnumEntry{ButtonId::NONE, TRACK_ACTION_ID_NONE},
    EnumEntry{ButtonId::LEFT, TRACK_ACTION_ID_NULL},
    EnumEntry{ButtonId::RIGHT, TRACK_ACTION_ID_NULL},
    EnumEntry{ButtonId::ENTER, TRACK_ACTION_ID_NULL},
    EnumEntry{ButtonId::EXIT, TRACK_ACTION_ID_NULL},
    EnumEntry{ButtonId::ENCODER_A_PUSH, TRACK_ACTION_ID_NULL},
    EnumEntry{ButtonId::ENCODER_B_PUSH, TRACK_ACTION_ID_NULL},
    EnumEntry{ButtonId::ENCODER_C_PUSH, TRACK_ACTION_ID_NULL},
    EnumEntry{ButtonId::ENCODER_D_PUSH, TRACK_ACTION_ID_NULL},
    EnumEntry{ButtonId::IFX_A_TOGGLE, TRACK_ACTION_ID_NULL},
    EnumEntry{ButtonId::TFX_A_TOGGLE, TRACK_ACTION_ID_NULL},
    EnumEntry{ButtonId::TRACK_1_EDIT, TRACK_ACTION_ID_NULL},
    EnumEntry{ButtonId::TRACK_1_PLAY_RECORD, TRACK_ACTION_ID_ENTER_RECORD_PLAY},
    EnumEntry{ButtonId::TRACK_1_STOP, TRACK_ACTION_ID_ENTER_STOP},
    EnumEntry{ButtonId::TRACK_2_EDIT, TRACK_ACTION_ID_NULL},
    EnumEntry{ButtonId::TRACK_2_PLAY_RECORD, TRACK_ACTION_ID_ENTER_RECORD_PLAY},
    EnumEntry{ButtonId::TRACK_2_STOP, TRACK_ACTION_ID_ENTER_STOP},
    EnumEntry{ButtonId::TRACK_3_EDIT, TRACK_ACTION_ID_NULL},
    EnumEntry{ButtonId::TRACK_3_PLAY_RECORD, TRACK_ACTION_ID_ENTER_RECORD_PLAY},
    EnumEntry{ButtonId::TRACK_3_STOP, TRACK_ACTION_ID_ENTER_STOP},
    EnumEntry{ButtonId::TRACK_4_EDIT, TRACK_ACTION_ID_NULL},
    EnumEntry{ButtonId::TRACK_4_PLAY_RECORD, TRACK_ACTION_ID_ENTER_RECORD_PLAY},
    EnumEntry{ButtonId::TRACK_4_STOP, TRACK_ACTION_ID_ENTER_STOP},
    EnumEntry{ButtonId::TRACK_5_EDIT, TRACK_ACTION_ID_NULL},
    EnumEntry{ButtonId::TRACK_5_PLAY_RECORD, TRACK_ACTION_ID_ENTER_RECORD_PLAY},
    EnumEntry{ButtonId::TRACK_5_STOP, TRACK_ACTION_ID_ENTER_STOP},
};

TrackActionId ButtonTrackActionConfigMap_Get(ButtonId id) {
  return button_track_action_map[id];
}
