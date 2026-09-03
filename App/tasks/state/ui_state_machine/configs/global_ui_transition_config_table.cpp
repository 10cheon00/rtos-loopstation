#include "global_ui_transition_config_table.h"

#include "enum_map.hpp"

static constexpr EnumMap<ButtonId, UiStateId> table{
    EnumEntry{ButtonId::NONE, UiStateId::NONE},
    EnumEntry{ButtonId::LEFT, UiStateId::NONE},
    EnumEntry{ButtonId::RIGHT, UiStateId::NONE},
    EnumEntry{ButtonId::ENTER, UiStateId::NONE},
    EnumEntry{ButtonId::EXIT, UiStateId::NONE},
    // 엔코더 푸시 버튼의 동작은 패널마다 다르므로 이곳에서 정의하지 않음
    EnumEntry{ButtonId::ENCODER_A_PUSH, UiStateId::NONE},
    EnumEntry{ButtonId::ENCODER_B_PUSH, UiStateId::NONE},
    EnumEntry{ButtonId::ENCODER_C_PUSH, UiStateId::NONE},
    EnumEntry{ButtonId::ENCODER_D_PUSH, UiStateId::NONE},
    EnumEntry{ButtonId::IFX_A_TOGGLE, UiStateId::NONE},
    EnumEntry{ButtonId::TFX_A_TOGGLE, UiStateId::NONE},
    EnumEntry{ButtonId::TRACK_1_EDIT, UiStateId::SETTING},
    EnumEntry{ButtonId::TRACK_1_PLAY_RECORD, UiStateId::NONE},
    EnumEntry{ButtonId::TRACK_1_STOP, UiStateId::NONE},
    EnumEntry{ButtonId::TRACK_2_EDIT, UiStateId::NONE},
    EnumEntry{ButtonId::TRACK_2_PLAY_RECORD, UiStateId::NONE},
    EnumEntry{ButtonId::TRACK_2_STOP, UiStateId::NONE},
    EnumEntry{ButtonId::TRACK_3_EDIT, UiStateId::NONE},
    EnumEntry{ButtonId::TRACK_3_PLAY_RECORD, UiStateId::NONE},
    EnumEntry{ButtonId::TRACK_3_STOP, UiStateId::NONE},
    EnumEntry{ButtonId::TRACK_4_EDIT, UiStateId::NONE},
    EnumEntry{ButtonId::TRACK_4_PLAY_RECORD, UiStateId::NONE},
    EnumEntry{ButtonId::TRACK_4_STOP, UiStateId::NONE},
    EnumEntry{ButtonId::TRACK_5_EDIT, UiStateId::NONE},
    EnumEntry{ButtonId::TRACK_5_PLAY_RECORD, UiStateId::NONE},
    EnumEntry{ButtonId::TRACK_5_STOP, UiStateId::NONE},
};

UiStateId GlobalUiTransitionConfigTable_Get(ButtonId id) {
  if (id <= ButtonId::NONE || id >= ButtonId::COUNT) {
    return UiStateId::NONE;
  }
  return table[id];
}
