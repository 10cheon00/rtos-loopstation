#include "button_to_track_action_map.hpp"

#include "button_id.hpp"
#include "enum_map.hpp"

namespace ButtonToTrackActionMap {

static constexpr EnumMap<ButtonId, TrackStateMachine::ActionId> button_track_action_map{
    EnumEntry{ButtonId::NONE, TrackStateMachine::ActionId::NONE},
    EnumEntry{ButtonId::LEFT, TrackStateMachine::ActionId::NONE},
    EnumEntry{ButtonId::RIGHT, TrackStateMachine::ActionId::NONE},
    EnumEntry{ButtonId::ENTER, TrackStateMachine::ActionId::NONE},
    EnumEntry{ButtonId::EXIT, TrackStateMachine::ActionId::NONE},
    EnumEntry{ButtonId::ENCODER_A_PUSH, TrackStateMachine::ActionId::NONE},
    EnumEntry{ButtonId::ENCODER_B_PUSH, TrackStateMachine::ActionId::NONE},
    EnumEntry{ButtonId::ENCODER_C_PUSH, TrackStateMachine::ActionId::NONE},
    EnumEntry{ButtonId::ENCODER_D_PUSH, TrackStateMachine::ActionId::NONE},
    EnumEntry{ButtonId::IFX_A_TOGGLE, TrackStateMachine::ActionId::NONE},
    EnumEntry{ButtonId::TFX_A_TOGGLE, TrackStateMachine::ActionId::NONE},
    EnumEntry{ButtonId::TRACK_1_EDIT, TrackStateMachine::ActionId::NONE},
    EnumEntry{ButtonId::TRACK_1_PLAY_RECORD, TrackStateMachine::ActionId::ENTER_RECORD_PLAY},
    EnumEntry{ButtonId::TRACK_1_STOP, TrackStateMachine::ActionId::ENTER_STOP},
    EnumEntry{ButtonId::TRACK_2_EDIT, TrackStateMachine::ActionId::NONE},
    EnumEntry{ButtonId::TRACK_2_PLAY_RECORD, TrackStateMachine::ActionId::ENTER_RECORD_PLAY},
    EnumEntry{ButtonId::TRACK_2_STOP, TrackStateMachine::ActionId::ENTER_STOP},
    EnumEntry{ButtonId::TRACK_3_EDIT, TrackStateMachine::ActionId::NONE},
    EnumEntry{ButtonId::TRACK_3_PLAY_RECORD, TrackStateMachine::ActionId::ENTER_RECORD_PLAY},
    EnumEntry{ButtonId::TRACK_3_STOP, TrackStateMachine::ActionId::ENTER_STOP},
    EnumEntry{ButtonId::TRACK_4_EDIT, TrackStateMachine::ActionId::NONE},
    EnumEntry{ButtonId::TRACK_4_PLAY_RECORD, TrackStateMachine::ActionId::ENTER_RECORD_PLAY},
    EnumEntry{ButtonId::TRACK_4_STOP, TrackStateMachine::ActionId::ENTER_STOP},
    EnumEntry{ButtonId::TRACK_5_EDIT, TrackStateMachine::ActionId::NONE},
    EnumEntry{ButtonId::TRACK_5_PLAY_RECORD, TrackStateMachine::ActionId::ENTER_RECORD_PLAY},
    EnumEntry{ButtonId::TRACK_5_STOP, TrackStateMachine::ActionId::ENTER_STOP},
};

TrackStateMachine::ActionId Get(ButtonId id) {
  return button_track_action_map[id];
}

}
