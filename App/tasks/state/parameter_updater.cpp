#include "parameter_updater.hpp"

#include <limits>

#include "knob_to_id_variant_map.hpp"
#include "loopstation_parameter_store.hpp"
#include "page.hpp"
#include "track_play_level.hpp"

void ParameterUpdater::Update(const StateEventVariant& variant) {
  std::visit([this](const auto& v) { Update(v); }, variant);
}

void ParameterUpdater::Update(const ButtonEvent& event) {
  if (event.state != ButtonState::PRESSED) {
    return;
  }

  switch (event.id) {
    case ButtonId::ENCODER_A_PUSH:
    case ButtonId::ENCODER_B_PUSH:
    case ButtonId::ENCODER_C_PUSH:
    case ButtonId::ENCODER_D_PUSH: {
      SlotIndex index = ToSlotIndex(event.id);
      Parameter& parameter = GetParameterFromCurrentPageAt(index);
      if (parameter.GetType() == ParameterType::TOGGLE) {
        parameter.Toggle();
      }
    } break;
    case ButtonId::IFX_A_TOGGLE: {
      Parameter& parameter =
          LoopstationStore::GetParameter(ParameterId::IFX_A_STATE);
      parameter.Toggle();
    } break;
    case ButtonId::TFX_A_TOGGLE: {
      Parameter& parameter =
          LoopstationStore::GetParameter(ParameterId::TFX_A_STATE);
      parameter.Toggle();
    } break;
    default:
      break;
  }
}

Parameter& ParameterUpdater::GetParameterFromCurrentPageAt(SlotIndex index) {
  Page& current_page =
      state_task_context.ui_state_machine.GetCurrentState()->GetCurrentPage();

  if (current_page.IsTypeAt<ParameterSlot>(index)) {
    ParameterId parameter_id =
        std::get<ParameterSlot>(current_page.GetAt(index)).GetParameterId();
    return LoopstationStore::GetParameter(parameter_id);
  }
  return LoopstationStore::GetParameter(ParameterId::NONE);
}

void ParameterUpdater::Update(const EncoderRotationEvent& event) {
  SlotIndex index = ToSlotIndex(event.id);
  Parameter& parameter = GetParameterFromCurrentPageAt(index);
  parameter.Add(event.delta);
}

void ParameterUpdater::Update(const AdcConversionEvent& event) {
  const KnobToIdVariantMap::IdVariant& id_variant =
      KnobToIdVariantMap::Get(event.id);
  // TODO: 노브 값을 파라미터 또는 트랙 설정에 저장하는 구조 재설계
  // 지금은 임시방편으로 붙였으나 깔끔한 구조가 아님.
  if (std::holds_alternative<ParameterId>(id_variant)) {
    Parameter& parameter =
        LoopstationStore::GetParameter(std::get<ParameterId>(id_variant));

    parameter.Set(MapRangeLinear(event.adc_value,
                                 std::numeric_limits<std::uint16_t>::min(),
                                 std::numeric_limits<std::uint16_t>::max(),
                                 parameter.GetMin(), parameter.GetMax()));
  } else if (std::holds_alternative<KnobToIdVariantMap::TrackAdcControl>(
                 id_variant)) {
    const KnobToIdVariantMap::TrackAdcControl& adc_control =
        std::get<KnobToIdVariantMap::TrackAdcControl>(id_variant);
    switch (adc_control.id) {
      case TrackSettingId::PLAY_LEVEL: {
        TrackPlayLevel& play_level =
            state_task_context.track_entry[ToIndex(adc_control.index)]
                .context.track_setting.GetPlayLevel();
        play_level.SetCurrent(MapRangeLinear(
            event.adc_value, std::numeric_limits<std::uint16_t>::min(),
            std::numeric_limits<std::uint16_t>::max(), play_level.GetMin(),
            play_level.GetMax()));
      } break;
      default:
        break;
    }
  }
}
