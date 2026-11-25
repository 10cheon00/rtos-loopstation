#ifndef PARAMETER_UPDATER_HPP
#define PARAMETER_UPDATER_HPP

#include "page.hpp"
#include "state_event_variant.hpp"

namespace ParameterUpdater {

struct FxSelection {
  Fx::Section section;
  Fx::Slot slot;
};

void Update(FxSelection& fx_selection, Page& current_page,
            StateEventVariant& variant);

}  // namespace ParameterUpdater

#endif