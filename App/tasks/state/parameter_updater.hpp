#ifndef PARAMETER_UPDATER_HPP
#define PARAMETER_UPDATER_HPP

#include "page.hpp"
#include "state_event_variant.hpp"

namespace ParameterUpdater {

struct FxSelection {
  Fx::Section section;
  Fx::Slot slot;
};

void Update(AdcConversionEvent& event);
void Update(FxSelection& fx_selection, Page& current_page,
            EncoderRotationEvent& event);
void Update(FxSelection& fx_selection, Page& current_page, ButtonEvent& event);
void ToggleFxStateFromFxSelection(FxSelection& fx_selection);
}  // namespace ParameterUpdater

#endif