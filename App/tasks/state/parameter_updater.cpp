#include "parameter_updater.hpp"

#include <limits>

#include "fx_parameter_id.hpp"
#include "fx_parameter_store.hpp"
#include "knob_to_parameter_map.hpp"
#include "slot_index.hpp"
#include "system_parameter_store.hpp"

namespace ParameterUpdater {

static void UpdateParameterFromButton(ButtonEvent& event,
                                      FxSelection& fx_selection,
                                      Page& current_page);
static void ToggleParameterMappedBySlotIndex(FxSelection& fx_selection,
                                             Page& current_page,
                                             SlotIndex index);
static void ToggleFxStateFromSectionAndSlot(FxSelection& fx_selection);
template <typename Operation>
static void UpdateFxParameterFromSectionAndSlotAndParameterId(
    FxSelection& fx_selection, Fx::ParameterId fx_parameter_id,
    Operation update_method);
static void UpdateParameterFromEncoderRotation(FxSelection& fx_selection,
                                               Page& current_page,
                                               EncoderRotationEvent& event);
static void AddParameterMappedBySlotIndex(FxSelection& fx_selection,
                                          Page& current_page, SlotIndex index,
                                          int32_t delta);
static void UpdateParameterFromAdc(AdcConversionEvent& event);

void Update(FxSelection& fx_selection, Page& current_page,
            StateEventVariant& variant) {
  if (std::holds_alternative<ButtonEvent>(variant)) {
    UpdateParameterFromButton(std::get<ButtonEvent>(variant), fx_selection,
                              current_page);
  } else if (std::holds_alternative<EncoderRotationEvent>(variant)) {
    UpdateParameterFromEncoderRotation(fx_selection, current_page,
                                       std::get<EncoderRotationEvent>(variant));
  } else if (std::holds_alternative<AdcConversionEvent>(variant)) {
    UpdateParameterFromAdc(std::get<AdcConversionEvent>(variant));
  }
}

/**
 * 버튼 입력은 IFX/TFX 토글, 엔코더 버튼만 파라미터 값을 변경한다.
 * */
static void UpdateParameterFromButton(ButtonEvent& event,
                                      FxSelection& fx_selection,
                                      Page& current_page) {
  if (event.state != ButtonState::PRESSED) {
    return;
  }

  switch (event.id) {
    case ButtonId::ENCODER_A_PUSH:
    case ButtonId::ENCODER_B_PUSH:
    case ButtonId::ENCODER_C_PUSH:
    case ButtonId::ENCODER_D_PUSH: {
      SlotIndex index = Utils::ToSlotIndex(event.id);
      ToggleParameterMappedBySlotIndex(fx_selection, current_page, index);
    } break;
    case ButtonId::IFX_A_TOGGLE:
      fx_selection.section = Fx::Section::IFX;
      fx_selection.slot = Fx::Slot::A;
      ToggleFxStateFromSectionAndSlot(fx_selection);
      break;
    case ButtonId::TFX_A_TOGGLE:
      fx_selection.section = Fx::Section::TFX;
      fx_selection.slot = Fx::Slot::A;
      ToggleFxStateFromSectionAndSlot(fx_selection);
      break;
    default:
      break;
  }
}

static void ToggleParameterMappedBySlotIndex(FxSelection& fx_selection,
                                             Page& current_page,
                                             SlotIndex index) {
  if (current_page.IsTypeAt<ParameterSlot>(index)) {
    ParameterSlot& parameter_slot =
        std::get<ParameterSlot>(current_page.GetAt(index));
    ParameterRef& parameter_ref = parameter_slot.GetParameterRef();
    if (std::holds_alternative<SystemParameterRef>(parameter_ref)) {
      SystemParameterRef system_parameter_ref =
          std::get<SystemParameterRef>(parameter_ref);
      Store::SystemParameterStore::GetInstance().WithParameterLocked(
          system_parameter_ref.id, [](auto& parameter) { parameter.Toggle(); });
    } else if (std::holds_alternative<FxParameterRef>(parameter_ref)) {
      FxParameterRef& fx_parameter_ref =
          std::get<FxParameterRef>(parameter_ref);
      UpdateFxParameterFromSectionAndSlotAndParameterId(
          fx_selection, fx_parameter_ref.id,
          [](auto& parameter) { parameter.Toggle(); });
    }
  }
}

static void ToggleFxStateFromSectionAndSlot(FxSelection& fx_selection) {
  Store::FxParameterStore::GetInstance().WithParameterLocked(
      fx_selection.section, fx_selection.slot,
      Fx::ParameterId::FX_SYSTEM_FX_STATE,
      [](auto& fx_parameter) { fx_parameter.Toggle(); });
}

template <typename Operation>
static void UpdateFxParameterFromSectionAndSlotAndParameterId(
    FxSelection& fx_selection, Fx::ParameterId fx_parameter_id,
    Operation update_method) {
  Store::FxParameterStore::GetInstance().WithParameterLocked(
      fx_selection.section, fx_selection.slot, fx_parameter_id, update_method);
}

/**
 * 엔코더 입력은 패널에 엔코더와 매핑된 파라미터가 있어야 하며 그 파라미터가
 * 토글형이라면 버튼입력과 회전입력으로 토글을 수행하고 노브형이라면
 * 회전입력을 받아 값을 변경한다.
 */
static void UpdateParameterFromEncoderRotation(FxSelection& fx_selection,
                                               Page& current_page,
                                               EncoderRotationEvent& event) {
  SlotIndex index = Utils::ToSlotIndex(event.id);
  AddParameterMappedBySlotIndex(fx_selection, current_page, index, event.delta);
}

static void AddParameterMappedBySlotIndex(FxSelection& fx_selection,
                                          Page& current_page, SlotIndex index,
                                          int32_t delta) {
  if (current_page.IsTypeAt<ParameterSlot>(index)) {
    ParameterSlot& parameter_slot =
        std::get<ParameterSlot>(current_page.GetAt(index));
    ParameterRef& parameter_ref = parameter_slot.GetParameterRef();
    if (std::holds_alternative<SystemParameterRef>(parameter_ref)) {
      SystemParameterRef system_parameter_ref =
          std::get<SystemParameterRef>(parameter_ref);
      Store::SystemParameterStore::GetInstance().WithParameterLocked(
          system_parameter_ref.id,
          [delta](auto& parameter) { parameter.Add(delta); });
    } else if (std::holds_alternative<FxParameterRef>(parameter_ref)) {
      FxParameterRef& fx_parameter_ref =
          std::get<FxParameterRef>(parameter_ref);
      UpdateFxParameterFromSectionAndSlotAndParameterId(
          fx_selection, fx_parameter_ref.id,
          [delta](auto& fx_parameter) { fx_parameter.Add(delta); });
    }
  }
}

/**
 * ADC입력은 16비트 해상도의 반환값을 파라미터 자료형에 맞도록 스케일한다.
 * 이 입력을 곧바로 FX 파라미터에 적용하지 않고 별도의 파라미터에 저장한다음,
 * 저장된 값을 FX 파라미터에 적용한다.
 */
static void UpdateParameterFromAdc(AdcConversionEvent& event) {
  Store::SystemParameterStore::GetInstance().WithParameterLocked(
      KnobToParameterMap::Get(event.id),
      [adc_value = event.adc_value](ParameterModel::Parameter& parameter) {
        auto& knob_value =
            parameter.GetValue<ParameterModel::UnsignedRangeValue>();
        parameter.Set(Utils::MapRangeLinear(
            adc_value, std::numeric_limits<std::uint16_t>::min(),
            std::numeric_limits<std::uint16_t>::max(), knob_value.min,
            knob_value.max));
      });
}

}  // namespace ParameterUpdater
