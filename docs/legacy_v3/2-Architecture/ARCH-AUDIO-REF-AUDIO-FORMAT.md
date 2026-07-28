---
title: 오디오 입출력 아키텍처 참조 - 오디오 포맷
version: 0.1.1
change_history:
  - date: 2026-07-11
    version: 0.1.0
    summary: 오디오 입출력 설계에서 참조할 공통 오디오 포맷과 버퍼 단위를 분리해 정리함
  - date: 2026-07-11
    version: 0.1.1
    summary: ARCH 기반 파일명 규칙에 맞춰 참조 링크를 갱신함
---

# 오디오 입출력 아키텍처 참조 - 오디오 포맷

이 문서는 [ARCH-AUDIO.md](./ARCH-AUDIO.md)에서 반복 참조하는 오디오 포맷, block 단위, 입출력 변환 기준을 정리한다.
요구사항별 설계 문서는 어떤 구조가 필요한지 설명하고, 이 문서는 그 구조가 공유하는 데이터 표현 기준을 설명한다.

## 1. 목표 포맷

| 항목 | 값 | 설명 |
| --- | --- | --- |
| sample rate | `44.1 kHz` | 입력, 저장, 재생, 출력의 기준 sample rate다. |
| 저장 파일 포맷 | WAV | 트랙 오디오를 파일로 저장할 때 사용하는 container다. |
| WAV 인코딩 | PCM linear | 압축 없이 sample을 시간 순서대로 저장한다. |
| 저장 bit depth | `16-bit` | SD 카드 저장량과 구현 단순성을 고려한 기본 저장 단위다. |
| 저장 channel | stereo | 내부 처리와 출력 경로의 L/R layout을 유지한다. |
| 저장 endianness | little-endian | WAV PCM 정수 field와 sample 저장 기준이다. |

## 2. 내부 처리 포맷

| 항목 | 결정 | 설명 |
| --- | --- | --- |
| 내부 sample type | `int32_t` | 입력 변환 이후 FX, 믹싱, gain 처리에 사용하는 공통 sample type이다. |
| 내부 channel layout | interleaved stereo | sample 순서는 `L, R, L, R, ...`이다. |
| mono 입력 처리 | L/R 복제 | 단일 마이크 입력은 좌우 채널에 같은 값을 넣어 stereo block으로 만든다. |
| clipping 처리 | 최종 출력 직전 제한 | 믹싱 이후 출력 또는 저장 format으로 변환하기 전에 범위를 제한한다. |
| gain/mixing headroom | 믹싱 단계에서 조정 | 여러 입력이 합산될 때 clipping이 발생하지 않도록 gain 정책을 둔다. |

## 3. 입출력 변환 기준

| 구분 | 기준 | 설명 |
| --- | --- | --- |
| 입력 수신 | I2S/SAI raw input word | 마이크 입력 raw word를 내부 `int32_t` sample로 변환한다. |
| 입력 channel 구성 | mono to stereo | 마이크 입력 sample을 내부 L/R channel에 복제한다. |
| 저장 변환 | internal `int32_t` to 16-bit PCM | 녹음 저장 직전에 16-bit little-endian PCM으로 변환한다. |
| 출력 변환 | internal `int32_t` to output I2S slot | 최종 mixed sample을 출력 장치 전송 slot에 맞춰 변환한다. |
| 출력 제한 | saturation 또는 limiter | 구체 정책은 오디오 입출력 설계의 미정 사항에서 결정한다. |

## 4. 버퍼 단위

| 항목 | 값 | 설명 |
| --- | --- | --- |
| audio frame | stereo L/R sample 묶음 | 한 시점의 L/R sample을 1 frame으로 본다. |
| audio block | `256 frame` | 실시간 오디오 처리의 기본 단위다. |
| audio block duration | 약 `5.80 ms` | `256 / 44100 * 1000` 기준이다. |
| internal audio block bytes | `2048 byte` | `256 frame * 2 channel * sizeof(int32_t)` 기준이다. |
| WAV audio block bytes | `1024 byte` | `256 frame * 2 channel * 16 bit / 8` 기준이다. |
| storage chunk | `1024 frame` | 저장 구조와 연동할 때 audio block 4개를 묶는 단위다. |
| storage chunk bytes | `4096 byte` | 16-bit stereo WAV PCM 기준이다. |
| storage chunk duration | 약 `23.22 ms` | `1024 / 44100 * 1000` 기준이다. |

## 5. 관련 상수 후보

| 상수 | 값 | 설명 |
| --- | --- | --- |
| `AUDIO_SAMPLE_RATE` | `44100` | 초당 audio frame 수 |
| `AUDIO_CHANNELS` | `2` | stereo L/R |
| `WAV_BITS_PER_SAMPLE` | `16` | WAV 저장 bit depth |
| `WAV_BYTES_PER_FRAME` | `4` | `2 channel * 16 bit / 8` |
| `INTERNAL_BYTES_PER_FRAME` | `8` | `2 channel * sizeof(int32_t)` |
| `AUDIO_BLOCK_FRAMES` | `256` | 실시간 처리 기본 단위 |
| `STORAGE_CHUNK_BLOCKS` | `4` | storage chunk를 구성하는 audio block 수 |
| `STORAGE_CHUNK_FRAMES` | `1024` | `256 frame * 4 block` |
| `STORAGE_CHUNK_BYTES` | `4096` | 16-bit stereo WAV PCM 기준 chunk byte 수 |
