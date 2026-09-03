#ifndef TRACK_STATE_MACHINE_CONTEXT_H
#define TRACK_STATE_MACHINE_CONTEXT_H

#include "cmsis_os2.h"

namespace TrackStateMachine {

struct Context {
  // TODO:
  // 트랙 상태 머신 전이 시 오디오 처리 태스크에 메시지를 보내야 하지 않을까?
  // 오디오 처리 태스크와 관련된 자원 추가하기
};

void InitContext(Context* context);

}  // namespace TrackStateMachine

#endif