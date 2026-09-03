#include "global_ui_transition_config_table.h"

#include "enum_map.hpp"

static constexpr EnumMap<ButtonId, UiStateMachine::Id> table{
    EnumEntry{ButtonId::NONE, UiStateMachine::Id::NONE},
    EnumEntry{ButtonId::LEFT, UiStateMachine::Id::NONE},
    EnumEntry{ButtonId::RIGHT, UiStateMachine::Id::NONE},
    EnumEntry{ButtonId::ENTER, UiStateMachine::Id::NONE},
    EnumEntry{ButtonId::EXIT, UiStateMachine::Id::NONE},
    // 엔코더 푸시 버튼의 동작은 패널마다 다르므로 이곳에서 정의하지 않음
    EnumEntry{ButtonId::ENCODER_A_PUSH, UiStateMachine::Id::NONE},
    EnumEntry{ButtonId::ENCODER_B_PUSH, UiStateMachine::Id::NONE},
    EnumEntry{ButtonId::ENCODER_C_PUSH, UiStateMachine::Id::NONE},
    EnumEntry{ButtonId::ENCODER_D_PUSH, UiStateMachine::Id::NONE},
    EnumEntry{ButtonId::IFX_A_TOGGLE, UiStateMachine::Id::NONE},
    EnumEntry{ButtonId::TFX_A_TOGGLE, UiStateMachine::Id::NONE},
    EnumEntry{ButtonId::TRACK_1_EDIT, UiStateMachine::Id::SETTING},
    EnumEntry{ButtonId::TRACK_1_PLAY_RECORD, UiStateMachine::Id::NONE},
    EnumEntry{ButtonId::TRACK_1_STOP, UiStateMachine::Id::NONE},
    EnumEntry{ButtonId::TRACK_2_EDIT, UiStateMachine::Id::NONE},
    EnumEntry{ButtonId::TRACK_2_PLAY_RECORD, UiStateMachine::Id::NONE},
    EnumEntry{ButtonId::TRACK_2_STOP, UiStateMachine::Id::NONE},
    EnumEntry{ButtonId::TRACK_3_EDIT, UiStateMachine::Id::NONE},
    EnumEntry{ButtonId::TRACK_3_PLAY_RECORD, UiStateMachine::Id::NONE},
    EnumEntry{ButtonId::TRACK_3_STOP, UiStateMachine::Id::NONE},
    EnumEntry{ButtonId::TRACK_4_EDIT, UiStateMachine::Id::NONE},
    EnumEntry{ButtonId::TRACK_4_PLAY_RECORD, UiStateMachine::Id::NONE},
    EnumEntry{ButtonId::TRACK_4_STOP, UiStateMachine::Id::NONE},
    EnumEntry{ButtonId::TRACK_5_EDIT, UiStateMachine::Id::NONE},
    EnumEntry{ButtonId::TRACK_5_PLAY_RECORD, UiStateMachine::Id::NONE},
    EnumEntry{ButtonId::TRACK_5_STOP, UiStateMachine::Id::NONE},
};

UiStateMachine::Id GlobalUiTransitionConfigTable_Get(ButtonId id) {
  return table[id];
}
