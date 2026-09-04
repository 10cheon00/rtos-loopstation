#ifndef SYSTEM_STATE_POINTER_MAP_HPP
#define SYSTEM_STATE_POINTER_MAP_HPP

#include "system_state.hpp"

namespace SystemStatePointerMap {
namespace SystemStateClass {

using namespace SystemStateMachine;

class NotInitState : public State {
 public:
  NotInitState()
      : State(Id::NOT_INITED, EnumEntry{ActionId::INITED, Id::RUNNING},
              EnumEntry{ActionId::ERROR, Id::ERROR}) {}

  virtual void OnEnter(Context& context) override {}
};

class RunningState : public State {
 public:
  RunningState() : State(Id::RUNNING, EnumEntry{ActionId::ERROR, Id::ERROR}) {}

  virtual void OnEnter(Context& context) override {}
};

class ErrorState : public State {
 public:
  ErrorState() : State(Id::ERROR, EnumEntry{ActionId::ERROR, Id::ERROR}) {}

  virtual void OnEnter(Context& context) override {}
};

}  // namespace SystemStateClass
}  // namespace SystemStatePointerMap

namespace SystemStatePointerMap {

using SystemStatePointer = SystemStateMachine::State*;

SystemStatePointer Get(SystemStateMachine::Id id);

}  // namespace SystemStatePointerMap

#endif
