#include "ui_transition_map.hpp"

#include "enum_map.hpp"

namespace UiTransitionMap {

static constexpr EnumMap<ButtonId, UiStateMachine::Id> map{
    {ButtonId::NONE, UiStateMachine::Id::NONE},
    {ButtonId::LEFT, UiStateMachine::Id::NONE},
    {ButtonId::RIGHT, UiStateMachine::Id::NONE},
    {ButtonId::ENTER, UiStateMachine::Id::NONE},
    {ButtonId::EXIT, UiStateMachine::Id::NONE},
    // 엔코더 푸시 버튼의 동작은 패널마다 다르므로 이곳에서 정의하지 않음
    {ButtonId::ENCODER_A_PUSH, UiStateMachine::Id::NONE},
    {ButtonId::ENCODER_B_PUSH, UiStateMachine::Id::NONE},
    {ButtonId::ENCODER_C_PUSH, UiStateMachine::Id::NONE},
    {ButtonId::ENCODER_D_PUSH, UiStateMachine::Id::NONE},
    {ButtonId::IFX_A_TOGGLE, UiStateMachine::Id::FX_EDIT},
    {ButtonId::TFX_A_TOGGLE, UiStateMachine::Id::FX_EDIT},
    {ButtonId::IFX_B_TOGGLE, UiStateMachine::Id::FX_EDIT},
    {ButtonId::TFX_B_TOGGLE, UiStateMachine::Id::FX_EDIT},
    {ButtonId::IFX_C_TOGGLE, UiStateMachine::Id::FX_EDIT},
    {ButtonId::TFX_C_TOGGLE, UiStateMachine::Id::FX_EDIT},
    {ButtonId::SETTING, UiStateMachine::Id::SETTING},
    {ButtonId::LOOP, UiStateMachine::Id::NONE},
    {ButtonId::TRACK_1_EDIT, UiStateMachine::Id::SETTING},
    {ButtonId::TRACK_1_PLAY_RECORD, UiStateMachine::Id::NONE},
    {ButtonId::TRACK_1_STOP, UiStateMachine::Id::NONE},
    {ButtonId::TRACK_2_EDIT, UiStateMachine::Id::NONE},
    {ButtonId::TRACK_2_PLAY_RECORD, UiStateMachine::Id::NONE},
    {ButtonId::TRACK_2_STOP, UiStateMachine::Id::NONE},
    {ButtonId::TRACK_3_EDIT, UiStateMachine::Id::NONE},
    {ButtonId::TRACK_3_PLAY_RECORD, UiStateMachine::Id::NONE},
    {ButtonId::TRACK_3_STOP, UiStateMachine::Id::NONE},
    {ButtonId::TRACK_4_EDIT, UiStateMachine::Id::NONE},
    {ButtonId::TRACK_4_PLAY_RECORD, UiStateMachine::Id::NONE},
    {ButtonId::TRACK_4_STOP, UiStateMachine::Id::NONE},
    {ButtonId::TRACK_5_EDIT, UiStateMachine::Id::NONE},
    {ButtonId::TRACK_5_PLAY_RECORD, UiStateMachine::Id::NONE},
    {ButtonId::TRACK_5_STOP, UiStateMachine::Id::NONE},
};

UiStateMachine::Id Get(ButtonId id) { return map[id]; }

}  // namespace UiTransitionMap
