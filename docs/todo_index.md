---
title: 문서 TODO 목록
version: 0.1.3
change_history:
  - date: 2026-07-10
    version: 0.1.0
    summary: docs 하위 문서의 TODO 항목을 모아보는 자동 생성 뷰를 추가함
  - date: 2026-07-10
    version: 0.1.1
    summary: 문서 TODO 목록을 최신 상태로 갱신함
  - date: 2026-07-10
    version: 0.1.2
    summary: 문서 TODO 목록을 최신 상태로 갱신함
  - date: 2026-07-11
    version: 0.1.3
    summary: 문서의 설계 및 설명 내용을 갱신함
---

# 문서 TODO 목록

이 문서는 `docs/` 하위 Markdown 문서에 남아 있는 TODO 항목을 모아 보여주는 자동 생성 뷰다.

- 생성 기준: `docs/**/*.md`, `docs/old/**`, `docs/todo_index.md` 제외
- TODO 개수: 60

| 파일 위치 | TODO 내용 |
| --- | --- |
| `docs/README.md:66` | 확인되지 않은 세부 사항은 `TODO:`로 남긴다. |
| `docs/old-2/architecture/message-driven_architecture.md:116` | 4. TODO: 아키텍처 시각화 및 태스크 요청 흐름 |
| `docs/old-2/architecture/messages/audio_task_messages.md:25` | `audio_dma_event_queue` \| TODO \| SAI DMA half/full event처럼 실시간성이 높은 이벤트를 수신한다. |
| `docs/old-2/architecture/messages/system_broadcast_messages.md:29` | 시스템 초기화 요청 \| 루프스테이션 상태 관리 태스크 \| `SYSTEM_RESET_REQUEST` \| TODO \| `SystemControlPayload` \| 상태, 오디오, 저장, 표시 관련 작업을 초기 상태로 되돌리도록 요청한다. |
| `docs/old-2/architecture/messages/system_broadcast_messages.md:30` | 시스템 정지 요청 \| 루프스테이션 상태 관리 태스크 \| `SYSTEM_STOP` \| TODO \| `SystemControlPayload` \| 치명적 오류, 사용자 초기화, 전원 종료 대응 시 각 태스크가 진행 중 작업을 멈추고 정리하도록 요청한다. |
| `docs/old-2/architecture/software_architecture.md:152` | TODO: 실제 RTOS 우선순위 값 |
| `docs/old-2/architecture/software_architecture.md:166` | TODO: 메시지 구조체 정의 |
| `docs/old-2/architecture/software_architecture.md:168` | TODO: 큐 길이와 버퍼 정책 |
| `docs/old-2/architecture/software_architecture.md:170` | TODO: 태스크 간 동기화 방식 |
| `docs/old-2/architecture/software_architecture.md:232` | TODO: 파일 포맷 |
| `docs/old-2/architecture/software_architecture.md:234` | TODO: 파일 flush 정책 |
| `docs/old-2/architecture/software_architecture.md:236` | TODO: 재생 스트리밍 버퍼 구조 |
| `docs/old-2/architecture/software_architecture.md:238` | TODO: SD 카드 오류 복구 방식 |
| `docs/old-2/architecture/storage_io_sequence.md:34` | TODO: 트랙 파일 포맷/저장 정책 문서 \| 트랙 파일명, 디렉터리, metadata, split 정책 |
| `docs/old-2/architecture/task_message_design.md:77` | TODO \| 아직 RTOS 기능과 구현 방식을 확정하지 않은 항목이다. |
| `docs/old-2/architecture/task_message_design.md:90` | 전체 태스크 대상 메시지 \| [system_broadcast_messages.md](./messages/system_broadcast_messages.md) \| TODO |
| `docs/old-2/audio/audio_data_format.md:185` | 10. todo: Bit depth 확장 계획 |
| `docs/old-2/audio/fx_design.md:80` | TODO: reverb 알고리즘 종류, 메모리 사용량을 정한다. |
| `docs/old-2/audio/fx_design.md:95` | TODO: delay buffer 크기와 feedback 구조를 정한다. |
| `docs/old-2/audio/fx_design.md:108` | TODO: all-pass filter stage 수와 feedback 구조를 정한다. |
| `docs/old-2/audio/fx_design.md:120` | TODO: delay line 길이, LFO 형태, stereo 처리 방식을 정한다. |
| `docs/old-2/audio/fx_design.md:132` | TODO: 최대 delay time, tempo sync 지원 여부, buffer 메모리 크기를 정한다. |
| `docs/old-2/audio/fx_design.md:160` | TODO: 위 FX 중 프로젝트에 추가할 항목을 선정한다. |
| `docs/old-2/audio/recording_flow.md:104` | TODO: 후보 길이를 고정 목록으로 둘지, 프로젝트 설정값으로 분리할지 결정한다. |
| `docs/old-2/audio/recording_flow.md:119` | TODO: 종료 요청이 후보 길이보다 늦게 들어온 경우, 가장 가까운 후보로 자를지 다음 후보 끝까지 연장할지 정책을 결정한다. |
| `docs/old-2/hardware/hardware_configuration.md:146` | TODO: 최종 PCB 또는 커넥터 핀맵 |
| `docs/old-2/planning/project_plan.md:133` | TODO: 허용 가능한 오디오 지연 시간 |
| `docs/old-2/planning/project_plan.md:134` | TODO: 허용 가능한 오디오 dropout 기준 |
| `docs/old-2/planning/project_plan.md:135` | TODO: 최대 녹음 길이 |
| `docs/old-2/planning/project_plan.md:136` | TODO: 파일 저장 성공률 또는 오류 복구 기준 |
| `docs/old-2/planning/requirements.md:71` | TODO: 파일명 규칙 |
| `docs/old-2/planning/requirements.md:72` | TODO: 메타데이터 저장 방식 |
| `docs/old-2/planning/requirements.md:100` | NFR-AUDIO-001 \| 오디오 지연 시간 목표 \| TODO: 허용 지연 시간 |
| `docs/old-2/planning/requirements.md:101` | NFR-AUDIO-002 \| 오디오 dropout 허용 기준 \| TODO: dropout 기준 |
| `docs/old-2/planning/requirements.md:102` | NFR-STORAGE-001 \| SD 카드 오류 처리 정책 \| TODO: 오류 처리 기준 |
| `docs/old-2/planning/requirements.md:125` | TODO: UI 최종 화면 레이아웃 |
| `docs/old-2/planning/user_scenarios.md:34` | TODO: SD 카드가 없거나 마운트 실패했을 때의 사용자 표시와 제한 동작 |
| `docs/old-2/storage/record_to_playback_transition_tba.md:144` | TODO: `RECORDING -> PLAYING`과 `OVERDUBBING -> PLAYING`에 서로 다른 buffer 정책을 둘지 결정한다. |
| `docs/old-2/storage/record_to_playback_transition_tba.md:146` | TODO: read-write handle을 유지한 상태에서 WAV header 갱신, `f_sync`, metadata commit 시점을 어떻게 둘지 결정한다. |
| `docs/old-2/storage/record_to_playback_transition_tba.md:148` | TODO: 첫 재생 chunk 준비 전까지 상태를 `PLAYING`으로 전환할지, 별도 `PREPARING_PLAYBACK` 상태를 둘지 결정한다. |
| `docs/old-2/storage/record_to_playback_transition_tba.md:150` | TODO: undo/redo 도입 시 take 파일, delta 파일, snapshot 파일 중 어떤 저장 정책을 사용할지 결정한다. |
| `docs/old-2/ui/ui_design.md:162` | TODO: 실제 버튼 배치와 MCP23017 GPIO 매핑을 UI 입력 이름에 연결 |
| `docs/old-2/ui/ui_design.md:183` | TODO: 실제 LED 종류와 MCP23017 GPIO 출력 매핑 |
| `docs/old-2/ui/ui_design.md:191` | TODO: 글자 크기가 큰 폰트는 아직 없음 |
| `docs/old-2/ui/ui_design.md:205` | TODO: 아이콘 목록 전체를 프로젝트에 포함시킬 수 없으므로 필요한 아이콘만 선별하여 별도의 에셋 파일로 만들어야 함 |
| `docs/old-2/ui/ui_design.md:239` | TODO: 추가바람... |
| `docs/old-2/ui/ui_design.md:245` | TODO: 수정되어야 함... |
| `docs/old-2/ui/ui_design.md:249` | TODO: 추가바람... |
| `docs/old-2/ui/ui_design.md:253` | TODO: 추가바람... |
| `docs/old-2/ui/ui_design.md:261` | TODO: 추가바람... |
| `docs/old-2/ui/ui_design.md:267` | TODO: 에러 목록 정의 및 화면 디자인 |
| `docs/old-2/verification/test_plan.md:62` | 7~8채널 확장 \| PA3~PA7 추가 입력이 가능하다. \| TODO \| |
| `docs/old-2/verification/test_plan.md:99` | TODO: 실제 통합 초기화 성공 로그 또는 체크 방법 |
| `docs/old-2/verification/test_plan.md:108` | TODO: DMA 적용 후 재검증 |
| `docs/old-2/verification/test_plan.md:117` | TODO: 오디오 버퍼와 SD write task 구현 후 테스트 |
| `docs/old-2/verification/test_plan.md:133` | TODO: 상태 전이별 LED/LCD 표시 검증 |
| `docs/old-2/verification/test_plan.md:145` | TODO: 오디오 dropout 허용 기준 |
| `docs/old-2/verification/test_plan.md:147` | TODO: 녹음 가능 최대 시간 |
| `docs/old-2/verification/test_plan.md:149` | TODO: SD 카드 오류 처리 성공 기준 |
| `docs/old-2/verification/test_plan.md:151` | TODO: UI 반응 시간 기준 |
