#include "track_state_pointer_map.hpp"

#include "enum_map.hpp"

namespace TrackStatePointerMap {
namespace TrackStateClass {

using namespace TrackStateMachine;

static IdleState idle_state;
static RecordingState recording_state;
static PlayingState playing_state;
static StoppedState stopped_state;
static OverdubbingState overdubbing_state;

}  // namespace TrackStateClass
}  // namespace TrackStatePointerMap

namespace TrackStatePointerMap {

using namespace TrackStateClass;

static constexpr EnumMap<Id, TrackStatePointer> track_state_map{
    EnumEntry{Id::NONE, nullptr},
    EnumEntry{Id::IDLE, &idle_state},
    EnumEntry{Id::RECORDING, &recording_state},
    EnumEntry{Id::STOPPED, &stopped_state},
    EnumEntry{Id::PLAYING, &playing_state},
    EnumEntry{Id::OVERDUBBING, &overdubbing_state},
};

TrackStatePointer Get(Id id) { return track_state_map[id]; }

}  // namespace TrackStatePointerMap