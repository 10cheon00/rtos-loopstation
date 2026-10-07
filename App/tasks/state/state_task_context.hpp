#ifndef STATE_TASK_CONTEXT_HPP
#define STATE_TASK_CONTEXT_HPP

#include <array>
#include <cstddef>

#include "FreeRTOS.h"
#include "button_id.hpp"
#include "track_config.h"
#include "track_state_machine.hpp"
#include "ui_state_machine.hpp"

struct TrackButtonIdSet {
  ButtonId play_record_id;
  ButtonId stop_id;
};

struct TrackEntry {
  TrackStateMachine::Context context;
  TrackStateMachine::StateMachine state_machine{context,
                                                TrackStateMachine::Id::IDLE};
  TrackButtonIdSet track_button_id_set;
};

struct StateTaskContext {
  osMessageQueueId_t state_event_queue;
  osMessageQueueId_t display_snapshot_mailbox;
  osMessageQueueId_t audio_event_snapshot_mailbox;

  UiStateMachine::Context ui_state_machine_context;
  UiStateMachine::StateMachine ui_state_machine{ui_state_machine_context,
                                                UiStateMachine::Id::HOME};

  std::array<TrackEntry, static_cast<std::size_t>(TRACK_COUNT)> track_entry;
};

#endif
