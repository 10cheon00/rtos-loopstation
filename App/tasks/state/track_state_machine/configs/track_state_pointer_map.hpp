#ifndef TRACK_STATE_POINTER_MAP_HPP
#define TRACK_STATE_POINTER_MAP_HPP

#include "track_state.hpp"

namespace TrackStatePointerMap {

using namespace TrackStateMachine;

using TrackStatePointer = State*;

TrackStatePointer Get(Id id);

}  // namespace TrackStatePointerMap

namespace TrackStatePointerMap {
namespace TrackStateClass {

using namespace TrackStateMachine;

class IdleState : public State {
 public:
  IdleState()
      : State(Id::IDLE, TransitionEntry{ActionId::NONE, Id::IDLE},
              TransitionEntry{ActionId::ENTER_RECORD_PLAY, Id::RECORDING},
              TransitionEntry{ActionId::ENTER_STOP, Id::IDLE}) {}

  void OnEnter(Context& context) override {}
};

class RecordingState : public State {
 public:
  RecordingState()
      : State(Id::RECORDING, TransitionEntry{ActionId::NONE, Id::RECORDING},
              TransitionEntry{ActionId::ENTER_RECORD_PLAY, Id::PLAYING},
              TransitionEntry{ActionId::ENTER_STOP, Id::STOPPED}) {}

  void OnEnter(Context& context) override {}
};

class PlayingState : public State {
 public:
  PlayingState()
      : State(Id::PLAYING, TransitionEntry{ActionId::NONE, Id::PLAYING},
              TransitionEntry{ActionId::ENTER_RECORD_PLAY, Id::OVERDUBBING},
              TransitionEntry{ActionId::ENTER_STOP, Id::STOPPED}) {}

  void OnEnter(Context& context) override {}
};

class StoppedState : public State {
 public:
  StoppedState()
      : State(Id::STOPPED, TransitionEntry{ActionId::NONE, Id::STOPPED},
              TransitionEntry{ActionId::ENTER_RECORD_PLAY, Id::PLAYING},
              TransitionEntry{ActionId::ENTER_STOP, Id::STOPPED}) {}

  void OnEnter(Context& context) override {}
};

class OverdubbingState : public State {
 public:
  OverdubbingState()
      : State(Id::OVERDUBBING, TransitionEntry{ActionId::NONE, Id::OVERDUBBING},
              TransitionEntry{ActionId::ENTER_RECORD_PLAY, Id::PLAYING},
              TransitionEntry{ActionId::ENTER_STOP, Id::STOPPED}) {}

  void OnEnter(Context& context) override {}
};

}  // namespace TrackStateClass
}  // namespace TrackStatePointerMap

#endif
