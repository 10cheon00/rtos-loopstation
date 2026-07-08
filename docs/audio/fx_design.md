---
title: FX 설계 문서
version: 0.1.2
change_history:
  - date: 2026-07-04
    version: 0.1.0
    summary: FX 설계 문서를 생성하고 필수 FX와 추가 후보 FX 기준을 정리함
  - date: 2026-07-07
    version: 0.1.1
    summary: 폴더 구조 개선에 맞춰 참조 문서 경로를 수정함
  - date: 2026-07-07
    version: 0.1.2
    summary: docs 폴더 재구성에 따라 문서 위치와 내부 링크를 갱신함
---

# FX 설계 문서

이 문서는 Loop Station 프로젝트에서 구현할 FX의 설명과 파라미터를 정리한다.

기준 자료는 `../references/RC-505_e02_W.pdf`의 20쪽 `Input FX/Track FX settings`와 21쪽 `FX Parameters`이다. 기존 기획 문서에서 필수 FX로 정한 항목은 `LPF`, `HPF`, `EQ`, `Reverb`이며, 이후 추가 후보는 `Flanger`, `Phaser`, `Chorus`, `Delay`이다.

RC-505 매뉴얼에서는 `LPF`와 `HPF`를 독립 FX 타입이 아니라 `FILTER` FX의 `Type` 파라미터 값으로 다룬다. 이 프로젝트에서는 조작 편의를 위해 사용자에게는 `LPF`, `HPF`를 각각 별도 FX처럼 노출하되, 내부 구현은 하나의 filter 계열 알고리즘으로 묶을 수 있다.

파라미터 이름 앞의 `@`는 IFX 노브 또는 TFX 노브로 조절 가능한 파라미터를 의미한다. 별도 값 범위가 정해지기 전까지 모든 수치형 파라미터는 `0~100` 범위를 사용하고, 기본값을 둔다.

## 1. FX 구분

| 구분 | 설명 |
| --- | --- |
| IFX | 오디오 입력에 적용하는 FX |
| TFX | 트랙 재생음 또는 최종 출력에 적용하는 FX |

필수 구현 FX는 IFX와 TFX 양쪽에서 모두 사용 가능한 FX다.

## 2. 필수 구현 FX

### 2.1 LPF

`LPF`는 cutoff frequency보다 높은 주파수 대역을 줄여 소리를 어둡고 부드럽게 만든다. RC-505 기준으로는 `FILTER` FX의 `Type = LPF`에 해당한다.

| 파라미터 | 범위 | 기본값 | 설명 |
| --- | --- | --- | --- |
| @Cutoff | 0~100 | 100 | 필터가 적용되기 시작하는 기준 주파수이다. |
| Resonance | 0~100 | 0 | cutoff 부근의 강조 정도이다. |
| Rate | 0~100 | 0 | cutoff 변조 속도이다. |
| Depth | 0~100 | 0 | cutoff 변조 깊이이다. |

### 2.2 HPF

`HPF`는 cutoff frequency보다 낮은 주파수 대역을 줄여 저역을 제거하고 소리를 가볍게 만든다. RC-505 기준으로는 `FILTER` FX의 `Type = HPF`에 해당한다.

| 파라미터 | 범위 | 기본값 | 설명 |
| --- | --- | --- | --- |
| @Cutoff | 0~100 | 0 | 필터가 적용되기 시작하는 기준 주파수이다. |
| Resonance | 0~100 | 0 | cutoff 부근의 강조 정도이다. |
| Rate | 0~100 | 0 | cutoff 변조 속도이다. |
| Depth | 0~100 | 0 | cutoff 변조 깊이이다. |

### 2.3 EQ

`EQ`는 여러 주파수 대역의 gain을 조절해 전체 음색을 보정한다. RC-505는 저역, 중저역, 중고역, 고역, 전체 레벨을 파라미터로 제공한다.

| 파라미터 | 범위 | 기본값 | 설명 |
| --- | --- | --- | --- |
| Low | -10~+10 | 0 | 저역 gain을 조절한다. |
| Low-Mid | -10~+10 | 0 | 중저역 gain을 조절한다. |
| High-Mid | -10~+10 | 0 | 중고역 gain을 조절한다. |
| High | -10~+10 | 0 | 고역 gain을 조절한다. |
| @Level | -10~+10 | 0 | EQ 적용 후 전체 출력 레벨을 조절한다. |

### 2.4 Reverb

`Reverb`는 소리에 잔향을 추가해 공간감을 만든다. RC-505 기준 파라미터는 잔향 시간과 이펙트 레벨이다.

| 파라미터 | 범위 | 기본값 | 설명 |
| --- | --- | --- | --- |
| Time | 0.0~10.0 | 3.0 | 잔향이 지속되는 시간을 조절한다. |
| @E. Level | 0~100 | 100 | 원음에 더해지는 reverb 성분의 출력 레벨을 조절한다. |

TODO: reverb 알고리즘 종류, 메모리 사용량을 정한다.

## 3. 추후 추가 후보 FX

### 3.1 Flanger

`Flanger`는 짧게 지연된 신호를 원음과 섞고 변조해 금속성의 움직이는 공명감을 만든다.

| 파라미터 | 범위 | 기본값 | 설명 |
| --- | --- | --- | --- |
| @Rate | 0~100 | 0 | 변조 속도를 조절한다. |
| @Depth | 0~100 | 0 | 변조 깊이를 조절한다. |
| @Resonance | 0~100 | 0 | 효과의 강조 정도를 조절한다. |
| @E. Level | 0~100 | 0 | 이펙트 성분의 출력 레벨을 조절한다. |

TODO: delay buffer 크기와 feedback 구조를 정한다.

### 3.2 Phaser

`Phaser`는 위상이 변한 신호를 원음에 섞어 물결치듯 움직이는 음색을 만든다.

| 파라미터 | 범위 | 기본값 | 설명 |
| --- | --- | --- | --- |
| @Rate | 0~100 | 0 | 변조 속도를 조절한다. |
| @Depth | 0~100 | 0 | 변조 깊이를 조절한다. |
| @Resonance | 0~100 | 0 | 효과의 강조 정도를 조절한다. |
| @E. Level | 0~100 | 0 | 이펙트 성분의 출력 레벨을 조절한다. |

TODO: all-pass filter stage 수와 feedback 구조를 정한다.

### 3.3 Chorus

`Chorus`는 원음에 약간 detune된 신호를 더해 소리의 폭과 깊이를 만든다.

| 파라미터 | 범위 | 기본값 | 설명 |
| --- | --- | --- | --- |
| @Rate | 0~100 | 0 | chorus 변조 속도를 조절한다. |
| @Depth | 0~100 | 0 | chorus 변조 깊이를 조절한다. |
| @E. Level | 0~100 | 0 | 이펙트 성분의 출력 레벨을 조절한다. |

TODO: delay line 길이, LFO 형태, stereo 처리 방식을 정한다.

### 3.4 Delay

`Delay`는 원음 뒤에 지연된 소리를 더해 반복감이나 공간감을 만든다.

| 파라미터 | 범위 | 기본값 | 설명 |
| --- | --- | --- | --- |
| @Time | 0~100 | 0 | delay 시간을 조절한다. |
| @Feedback | 0~100 | 0 | delay 반복 횟수 또는 되먹임 양을 조절한다. |
| @E. Level | 0~100 | 0 | delay 성분의 출력 레벨을 조절한다. |

TODO: 최대 delay time, tempo sync 지원 여부, buffer 메모리 크기를 정한다.

## 4. RC-505의 기타 FX 참고 목록

아래 항목은 RC-505 매뉴얼 20쪽에 포함되어 있지만, 현재 프로젝트의 필수 또는 추후 후보로 확정하지 않은 FX이다.

| FX | 설명 |
| --- | --- |
| BPF | `FILTER`의 한 종류로, cutoff 주변 대역만 남기고 나머지를 줄인다. |
| SYNTH | 입력 신호를 바탕으로 synthesizer 성격의 소리를 만든다. |
| LO-FI | bit depth나 sample rate를 낮춰 거친 질감을 만든다. |
| GUITAR TO BASS | guitar 입력을 bass처럼 변환한다. |
| TRANSPOSE | FX가 켜졌을 때 pitch를 반음 단위로 이동한다. |
| ROBOT | 고정 pitch 성격의 robot voice를 만든다. |
| VOCAL DIST | vocal에 distortion을 적용한다. |
| VOCODER | 입력 오디오를 modulation 신호로 사용해 vocoder 음색을 만든다. |
| DYNAMICS | 음량 차이를 줄여 더 일정한 level로 만든다. |
| ISOLATOR | LOW, MID, HIGH 대역 중 지정한 영역을 줄인다. |
| OCTAVE | 1 octave 또는 2 octave 낮은 음을 추가한다. |
| PAN | stereo 위치를 이동시킨다. |
| SLICER | 소리를 반복적으로 잘라 rhythm backing처럼 만든다. |
| TAPE ECHO | tape echo 성격의 delay를 만든다. |
| GRANULAR DELAY | 짧은 입력 구간을 반복해 granular한 질감을 만든다. |
| BEAT REPEAT | 트랙을 beat에 맞춰 반복 재생한다. Track FX 전용이다. |
| BEAT SHIFT | 트랙 재생 위치를 beat 단위로 이동한다. Track FX 전용이다. |
| BEAT SCATTER | 트랙을 beat에 맞춰 scrub playback한다. Track FX 전용이다. |
| VINYL FLICK | record 회전을 조작하는 듯한 효과를 만든다. Track FX 전용이다. |

TODO: 위 FX 중 프로젝트에 추가할 항목을 선정한다.
