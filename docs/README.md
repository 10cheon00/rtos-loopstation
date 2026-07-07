# Loop Station 기획 문서

이 폴더는 Loop Station 프로젝트를 기획, 설계, 검증 관점에서 읽기 좋게 정리한 문서 묶음이다. 기존 `docs/project_summary.md`, `docs/software_design.md`, `docs/hardware_wiring.md`, `docs/peripheral_connections.md`에 흩어져 있던 내용을 문서 목적별로 다시 나누었다.

## 문서 목록

| 문서 | 목적 |
| --- | --- |
| [project_plan.md](./project_plan.md) | 프로젝트 목표, 범위, 성공 기준을 정의한다. |
| [requirements.md](./requirements.md) | 기능/비기능/하드웨어 요구사항을 정리한다. |
| [user_scenarios.md](./user_scenarios.md) | 사용자가 시스템을 조작하는 주요 흐름을 정리한다. |
| [ui_design.md](./ui_design.md) | LCD 패널, 버튼, 엔코더, LED 표시 정책을 정리한다. |
| [fx_design.md](./fx_design.md) | 필수 FX와 추후 FX 후보의 설명 및 파라미터를 정리한다. |
| [audio_data_format.md](./audio_data_format.md) | 저장/처리/입출력에 사용할 오디오 데이터 포맷 기준을 정리한다. |
| [software_architecture.md](./software_architecture.md) | RTOS 태스크, 상태 머신, 오디오 처리 구조를 정리한다. |
| [layered_architecture.md](./layered_architecture.md) | 사용자 이벤트와 하드웨어 데이터가 시스템 계층을 통과하는 책임 경계를 정리한다. |
| [hardware_configuration.md](./hardware_configuration.md) | 현재 검증된 모듈과 STM32 결선을 정리한다. |
| [test_plan.md](./test_plan.md) | 모듈 테스트와 통합 테스트 계획을 정리한다. |

## 작성 원칙

- 기존 문서에서 확인할 수 있는 내용만 확정 사항으로 적는다.
- 확인되지 않은 세부 사항은 `TODO:`로 남긴다.
- 실제 결선 기준은 `docs/hardware_wiring.md`를 우선 참고한다.
- 개발 중 의사결정 기록은 `docs/peripheral_connections.md`를 참고한다.
