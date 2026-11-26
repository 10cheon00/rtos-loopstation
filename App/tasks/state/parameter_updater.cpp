#include "parameter_updater.hpp"

#include <cstdbool>
#include <limits>

#include "fx_parameter_id.hpp"
#include "fx_parameter_store.hpp"
#include "knob_to_parameter_map.hpp"
#include "parameter_id.hpp"
#include "slot_index.hpp"
#include "system_parameter_store.hpp"

namespace ParameterUpdater {

static bool IsEncoderButtonEvent(ButtonEvent& event);
static bool IsParameterSlot(PageSlotVariant& variant);
template <typename Handler>
static void ModifyParameterInPageSlotVariant(FxSelection& fx_selection,
                                             PageSlotVariant& page_slot_variant,
                                             Handler handler);

/**
 * ADC입력은 16비트 해상도의 반환값을 파라미터 자료형에 맞도록 스케일한다.
 * 이 입력을 곧바로 FX 파라미터에 적용하지 않고 별도의 파라미터에 저장한다음,
 * 저장된 값을 FX 파라미터에 적용한다.
 */
void Update(AdcConversionEvent& event) {
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

void Update(FxSelection& fx_selection, Page& current_page,
            EncoderRotationEvent& event) {
  PageSlotVariant& page_slot_variant =
      current_page.GetAt(Utils::ToSlotIndex(event.id));
  if (IsParameterSlot(page_slot_variant)) {
    ModifyParameterInPageSlotVariant(
        fx_selection, page_slot_variant,
        [&event = event](auto& parameter) { parameter.Add(event.delta); });
  }
}

void Update(FxSelection& fx_selection, Page& current_page, ButtonEvent& event) {
  // 엔코더 버튼인 경우에 파라미터를 수정
  if (IsEncoderButtonEvent(event)) {
    PageSlotVariant& page_slot_variant =
        current_page.GetAt(Utils::ToSlotIndex(event.id));
    if (IsParameterSlot(page_slot_variant)) {
      ModifyParameterInPageSlotVariant(
          fx_selection, page_slot_variant,
          [](auto& parameter) { parameter.Toggle(); });
    }
  }
}

static bool IsEncoderButtonEvent(ButtonEvent& event) {
  return event.id == ButtonId::ENCODER_A_PUSH ||
         event.id == ButtonId::ENCODER_B_PUSH ||
         event.id == ButtonId::ENCODER_C_PUSH ||
         event.id == ButtonId::ENCODER_D_PUSH;
}

static bool IsParameterSlot(PageSlotVariant& variant) {
  return std::holds_alternative<ParameterSlot>(variant);
}

template <typename Handler>
static void ModifyParameterInPageSlotVariant(FxSelection& fx_selection,
                                             PageSlotVariant& page_slot_variant,
                                             Handler handler) {
  ParameterRef& parameter_ref =
      std::get<ParameterSlot>(page_slot_variant).GetParameterRef();

  std::visit(
      [&section = fx_selection.section, &slot = fx_selection.slot,
       &handler = handler](auto& parameter_ref) {
        using Ref = std::decay_t<decltype(parameter_ref)>;
        if constexpr (std::is_same_v<Ref, SystemParameterRef>) {
          ParameterModel::Id id = parameter_ref.id;
          Store::SystemParameterStore::GetInstance().WithParameterLocked(
              id, handler);

        } else if constexpr (std::is_same_v<Ref, FxParameterRef>) {
          Fx::ParameterId id = parameter_ref.id;

          Store::FxParameterStore::GetInstance().WithParameterLocked(
              section, slot, id, handler);
        }
      },
      parameter_ref);
}

void ToggleFxStateFromFxSelection(FxSelection& fx_selection) {
  Store::FxParameterStore::GetInstance().WithParameterLocked(
      fx_selection.section, fx_selection.slot,
      Fx::ParameterId::FX_SYSTEM_FX_STATE,
      [](auto& fx_state_parameter) { fx_state_parameter.Toggle(); });
}

}  // namespace ParameterUpdater
