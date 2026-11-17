#include "button_to_track_action_map.hpp"

#include "button_id.hpp"
#include "enum_map.hpp"

namespace ButtonToTrackActionMap {

static constexpr EnumMap<ButtonId, TrackStateMachine::ActionId> button_track_action_map{
    {ButtonId::NONE, TrackStateMachine::ActionId::NONE},
    {ButtonId::LEFT, TrackStateMachine::ActionId::NONE},
    {ButtonId::RIGHT, TrackStateMachine::ActionId::NONE},
    {ButtonId::ENTER, TrackStateMachine::ActionId::NONE},
    {ButtonId::EXIT, TrackStateMachine::ActionId::NONE},
    {ButtonId::ENCODER_A_PUSH, TrackStateMachine::ActionId::NONE},
    {ButtonId::ENCODER_B_PUSH, TrackStateMachine::ActionId::NONE},
    {ButtonId::ENCODER_C_PUSH, TrackStateMachine::ActionId::NONE},
    {ButtonId::ENCODER_D_PUSH, TrackStateMachine::ActionId::NONE},
    {ButtonId::IFX_A_TOGGLE, TrackStateMachine::ActionId::NONE},
    {ButtonId::TFX_A_TOGGLE, TrackStateMachine::ActionId::NONE},
    {ButtonId::TRACK_1_EDIT, TrackStateMachine::ActionId::NONE},
    {ButtonId::TRACK_1_PLAY_RECORD, TrackStateMachine::ActionId::ENTER_RECORD_PLAY},
    {ButtonId::TRACK_1_STOP, TrackStateMachine::ActionId::ENTER_STOP},
    {ButtonId::TRACK_2_EDIT, TrackStateMachine::ActionId::NONE},
    {ButtonId::TRACK_2_PLAY_RECORD, TrackStateMachine::ActionId::ENTER_RECORD_PLAY},
    {ButtonId::TRACK_2_STOP, TrackStateMachine::ActionId::ENTER_STOP},
    {ButtonId::TRACK_3_EDIT, TrackStateMachine::ActionId::NONE},
    {ButtonId::TRACK_3_PLAY_RECORD, TrackStateMachine::ActionId::ENTER_RECORD_PLAY},
    {ButtonId::TRACK_3_STOP, TrackStateMachine::ActionId::ENTER_STOP},
    {ButtonId::TRACK_4_EDIT, TrackStateMachine::ActionId::NONE},
    {ButtonId::TRACK_4_PLAY_RECORD, TrackStateMachine::ActionId::ENTER_RECORD_PLAY},
    {ButtonId::TRACK_4_STOP, TrackStateMachine::ActionId::ENTER_STOP},
    {ButtonId::TRACK_5_EDIT, TrackStateMachine::ActionId::NONE},
    {ButtonId::TRACK_5_PLAY_RECORD, TrackStateMachine::ActionId::ENTER_RECORD_PLAY},
    {ButtonId::TRACK_5_STOP, TrackStateMachine::ActionId::ENTER_STOP},
};

TrackStateMachine::ActionId Get(ButtonId id) {
  return button_track_action_map[id];
}

}
