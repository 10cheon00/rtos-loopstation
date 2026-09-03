#include "global_ui_transition_config_table.h"

#include "enum_map.hpp"

static constexpr EnumMap<ButtonId, UiStateId, BUTTON_ID_COUNT> table{
    EnumEntry{BUTTON_ID_NONE, static_cast<UiStateId>(ID_NONE)},
    EnumEntry{BUTTON_ID_NULL, static_cast<UiStateId>(ID_NULL)},
    EnumEntry{BUTTON_ID_LEFT, UiStateId::NONE},
    EnumEntry{BUTTON_ID_RIGHT, UiStateId::NONE},
    EnumEntry{BUTTON_ID_ENTER, UiStateId::NONE},
    EnumEntry{BUTTON_ID_EXIT, UiStateId::NONE},
    // 엔코더 푸시 버튼의 동작은 패널마다 다르므로 이곳에서 정의하지 않음
    EnumEntry{BUTTON_ID_ENCODER_A_PUSH, UiStateId::NONE},
    EnumEntry{BUTTON_ID_ENCODER_B_PUSH, UiStateId::NONE},
    EnumEntry{BUTTON_ID_ENCODER_C_PUSH, UiStateId::NONE},
    EnumEntry{BUTTON_ID_ENCODER_D_PUSH, UiStateId::NONE},
    EnumEntry{BUTTON_ID_IFX_A_TOGGLE, UiStateId::NONE},
    EnumEntry{BUTTON_ID_TFX_A_TOGGLE, UiStateId::NONE},
    EnumEntry{BUTTON_ID_TRACK_1_EDIT, UiStateId::SETTING},
    EnumEntry{BUTTON_ID_TRACK_1_PLAY_RECORD, UiStateId::NONE},
    EnumEntry{BUTTON_ID_TRACK_1_STOP, UiStateId::NONE},
    EnumEntry{BUTTON_ID_TRACK_2_EDIT, UiStateId::NONE},
    EnumEntry{BUTTON_ID_TRACK_2_PLAY_RECORD, UiStateId::NONE},
    EnumEntry{BUTTON_ID_TRACK_2_STOP, UiStateId::NONE},
    EnumEntry{BUTTON_ID_TRACK_3_EDIT, UiStateId::NONE},
    EnumEntry{BUTTON_ID_TRACK_3_PLAY_RECORD, UiStateId::NONE},
    EnumEntry{BUTTON_ID_TRACK_3_STOP, UiStateId::NONE},
    EnumEntry{BUTTON_ID_TRACK_4_EDIT, UiStateId::NONE},
    EnumEntry{BUTTON_ID_TRACK_4_PLAY_RECORD, UiStateId::NONE},
    EnumEntry{BUTTON_ID_TRACK_4_STOP, UiStateId::NONE},
    EnumEntry{BUTTON_ID_TRACK_5_EDIT, UiStateId::NONE},
    EnumEntry{BUTTON_ID_TRACK_5_PLAY_RECORD, UiStateId::NONE},
    EnumEntry{BUTTON_ID_TRACK_5_STOP, UiStateId::NONE},
};

UiStateId GlobalUiTransitionConfigTable_Get(ButtonId id) {
  if (id <= BUTTON_ID_NONE || id >= BUTTON_ID_COUNT) {
    return UiStateId::NONE;
  }
  return table[id];
}
