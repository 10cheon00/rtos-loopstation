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
    {Id::NONE, nullptr},
    {Id::IDLE, &idle_state},
    {Id::RECORDING, &recording_state},
    {Id::STOPPED, &stopped_state},
    {Id::PLAYING, &playing_state},
    {Id::OVERDUBBING, &overdubbing_state},
};

TrackStatePointer Get(Id id) { return track_state_map[id]; }

}  // namespace TrackStatePointerMap