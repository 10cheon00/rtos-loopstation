#ifndef PARAMETER_UPDATER_HPP
#define PARAMETER_UPDATER_HPP

#include "parameter.hpp"
#include "slot_index.hpp"
#include "state_event_variant.hpp"
#include "state_task_context.hpp"
#include "track_config.h"
#include "track_state_machine.hpp"
#include "ui_state_machine.hpp"

class ParameterUpdater {
 public:
  ParameterUpdater(StateTaskContext& state_task_context)
      : state_task_context(state_task_context) {}
  void Update(const StateEventVariant& variant);

 private:
  void Update(const ButtonEvent& event);
  void Update(const EncoderRotationEvent& event);
  void Update(const AdcConversionEvent& event);

  Parameter& GetParameterFromCurrentPageAt(SlotIndex index);

 private:
  StateTaskContext& state_task_context;
};

#endif
