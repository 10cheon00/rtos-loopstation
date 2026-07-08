---
title: 오디오 데이터 포맷 문서
version: 0.1.2
change_history:
  - date: 2026-07-07
    version: 0.1.0
    summary: 오디오 데이터 포맷 문서를 생성하고 WAV PCM linear 저장 기준을 정리함
  - date: 2026-07-07
    version: 0.1.1
    summary: 폴더 구조 개선에 맞춰 참조 문서 경로를 수정함
  - date: 2026-07-07
    version: 0.1.2
    summary: docs 폴더 재구성에 따라 문서 위치와 내부 링크를 갱신함
---

# 오디오 데이터 포맷 문서

이 문서는 Loop Station 프로젝트에서 사용할 오디오 데이터 포맷을 정의하기 위한 기준 문서다. 오디오 믹싱, FX 처리, SD 카드 저장 성능, 구현 단순성을 기준으로 내부 처리 포맷과 저장 포맷을 나누어 결정한다.

상세 수치와 변환 정책은 구현 전에 확정한다.

# 1. 기준 자료

| 자료 | 확인할 내용 |
| --- | --- |
| `../references/RC-505_e02_W.pdf` | 상용 루프스테이션의 일반적인 WAV 저장 포맷 참고 |
| `rtos-loopstation.ioc` | 현재 SAI1, SDMMC1, clock 설정 |
| [hardware_configuration.md](../hardware/hardware_configuration.md) | 오디오 입출력 모듈과 peripheral 연결 |
| [software_architecture.md](../architecture/software_architecture.md) | 오디오 처리 태스크, 버퍼, 저장장치 입출력 구조 |
| [fx_design.md](./fx_design.md) | FX 처리에서 요구하는 내부 sample 표현 |

# 2. 목표 포맷

프로젝트의 오디오 포맷 및 기타 설정값들은 다음과 같다.

| 항목 | 값 | 
| --- | --- | 
| 파일 포맷 | WAV | 
| WAV 인코딩 | PCM linear | 
| 샘플링레이트 | 44.1 kHz|
| Bit depth | 16-bit |
| 채널 | Stereo |
| Endianness | little-endian |

# 3. 내부 처리 포맷

SAI로 들어온 오디오 데이터를 시스템 내부에서 어떤 타입과 범위로 다룰지 정의한다.

| 항목 | 결정 |
| --- | --- |
| 내부 sample type | `int32_t` |
| 내부 channel layout | interleaved stereo |
| sample value range | `int16_t` WAV 값을 `int32_t`로 확장해 사용 |
| mono 입력 처리 | L/R 복제 |
| clipping 처리 | 최종 출력 직전에 `int16_t` 범위로 saturation |
| gain/mixing headroom | 믹싱 시 트랙 수에 따라 gain 감소 |

# 4. SAI 입출력 변환 정책

INMP441 입력, DAC 출력, 내부 sample format 사이의 변환 규칙을 정의한다.

## 4.1 입력 변환

| 단계 | 정책 |
| --- | --- |
| SAI receive frame | `SAI1 Block B`를 I2S standard, synchronous slave RX, stereo mode로 사용한다. `.ioc` 기준 sample rate는 `44.1 KHz`, data size는 `SAI_PROTOCOL_DATASIZE_32BIT`이며, 한 frame은 left/right slot 2개로 다룬다. |
| INMP441 sample alignment | INMP441의 `SD`는 `PE3 / SAI1_SD_B`로 수신한다. `BCK/SCK`는 `PE5 / SAI1_SCK_A`, `WS/LRCK`는 `PE4 / SAI1_FS_A`를 공유한다. 수신 word는 32-bit slot 안의 유효 sample을 상위 bit 기준으로 정렬해 해석한다. |
| 내부 sample 변환 | SAI 수신 raw word를 `int32_t` 내부 sample로 변환한다. 현재 테스트 코드 기준으로는 32-bit word에서 하위 8 bit를 버리고 24-bit 유효 sample을 얻는 구조를 사용한다. 저장 포맷이 16-bit이므로 WAV 저장 직전에는 `int16_t` 범위로 축소한다. |
| stereo 구성 | INMP441은 단일 마이크 입력이므로 내부 interleaved stereo buffer에는 같은 sample을 L/R에 복제한다. 순서는 `L, R, L, R, ...`로 둔다. |

## 4.2 출력 변환

| 단계 | 정책 |
| --- | --- |
| 내부 sample 입력 | 내부 처리는 `int32_t` sample, interleaved stereo layout을 기준으로 한다. 출력 변환 입력 순서는 `L, R, L, R, ...`이다. |
| DAC 전송 sample width | `.ioc`와 생성 코드 기준 `SAI1 Block A`는 I2S standard, master TX, stereo mode, sample rate `44.1 KHz`, `SAI_PROTOCOL_DATASIZE_24BIT`로 전송한다. |
| SAI transmit frame 구성 | 내부 `int32_t` sample을 DAC용 24-bit I2S slot에 맞춰 배치해 `SAI1 Block A`로 전송한다. `DIN`은 `PE6 / SAI1_SD_A`, `BCK/SCK`는 `PE5`, `WS/LRCK`는 `PE4`를 사용한다. |
| clipping/saturation | 출력 직전에 내부 sample을 `int16_t` 범위로 saturation한 뒤, DAC 전송 word에 맞춰 확장한다. 저장 포맷과 같은 16-bit linear 기준을 출력 기준값으로 둔다. |

# 5. WAV 저장 포맷

SD 카드에 저장할 WAV 파일의 구조를 정의한다.

| 항목 | 결정 |
| --- | --- |
| RIFF/WAVE header | 표준 RIFF WAVE header를 사용한다. `ChunkID="RIFF"`, `Format="WAVE"`로 기록하고, 모든 multi-byte 정수는 little-endian으로 저장한다. |
| `fmt ` chunk | PCM 기본 `fmt ` chunk를 사용한다. `AudioFormat=1`, `NumChannels=2`, `SampleRate=44100`, `BitsPerSample=16`, `BlockAlign=4`, `ByteRate=176400`으로 기록한다. |
| `data` chunk | interleaved stereo PCM sample을 `L, R, L, R, ...` 순서로 저장한다. sample 하나는 signed 16-bit little-endian이다. |
| 파일 크기 갱신 방식 | 녹음 중 누적한 PCM data byte 수를 기준으로 녹음 종료 시 `RIFF ChunkSize = 36 + Subchunk2Size`, `data Subchunk2Size = PCM data byte 수`를 갱신한다. |
| 녹음 중 header 임시값 처리 | 파일 생성 직후 44 byte WAV header를 먼저 기록하되, `ChunkSize`와 `Subchunk2Size`는 `0`으로 둔다. 이후 PCM data를 append한다. |
| 녹음 종료 시 finalize 방식 | `f_lseek()`로 header 위치로 돌아가 최종 `ChunkSize`와 `Subchunk2Size`를 다시 쓰고 `f_sync()` 또는 `f_close()`로 저장을 확정한다. |

WAV PCM header의 필드는 다음 값을 기준으로 한다.

| 필드 | 값 |
| --- | --- |
| `ChunkID` | `"RIFF"` |
| `ChunkSize` | `36 + Subchunk2Size` |
| `Format` | `"WAVE"` |
| `Subchunk1ID` | `"fmt "` |
| `Subchunk1Size` | `16` |
| `AudioFormat` | `1` (`PCM`) |
| `NumChannels` | `2` |
| `SampleRate` | `44100` |
| `ByteRate` | `44100 * 2 * 16 / 8 = 176400` |
| `BlockAlign` | `2 * 16 / 8 = 4` |
| `BitsPerSample` | `16` |
| `Subchunk2ID` | `"data"` |
| `Subchunk2Size` | PCM data byte 수 |

# 6. 버퍼 단위

오디오 처리, SD 저장, 재생 스트리밍에서 사용할 buffer 단위를 정의한다.

| 항목 | 결정 |
| --- | --- |
| 오디오 frame 정의 | 한 시점의 stereo sample 묶음을 1 frame으로 둔다. 즉 `L + R`이 1 frame이다. 내부 처리에서는 `int32_t L + int32_t R = 8 byte/frame`, WAV 저장에서는 `int16_t L + int16_t R = 4 byte/frame`이다. |
| audio block 크기 | `256 frame`을 기본 처리 단위로 둔다. 44.1 kHz 기준 약 `5.80 ms`이며, 내부 처리 버퍼는 `256 * 8 = 2048 byte`, WAV PCM 변환 후에는 `256 * 4 = 1024 byte`이다. |
| DMA half/full callback 단위 | DMA 적용 시 circular buffer 전체를 `512 frame`으로 두고, half/full callback마다 `256 frame`씩 처리한다. 따라서 callback 1회가 audio block 1개와 대응된다. |
| SD write chunk 크기 | FatFs write는 audio block 4개를 모아 `1024 frame = 4096 byte` 단위로 수행한다. WAV PCM 기준 약 `23.22 ms` 분량이다. |
| SD read chunk 크기 | SD read도 write와 동일하게 `1024 frame = 4096 byte` 단위로 수행한다. 읽은 데이터는 `256 frame` audio block 4개로 나누어 재생/믹싱 계층에 전달한다. |
| queue에 전달할 단위 | 실시간 오디오 처리 queue에는 `256 frame` audio block descriptor를 전달한다. SD 저장/읽기 queue에는 필요에 따라 `4096 byte` storage chunk descriptor를 전달한다. |

버퍼 크기 기준값은 다음과 같다.

| 상수 | 값 | 설명 |
| --- | --- | --- |
| `AUDIO_SAMPLE_RATE` | `44100` | 초당 audio frame 수 |
| `AUDIO_CHANNELS` | `2` | stereo L/R |
| `WAV_BITS_PER_SAMPLE` | `16` | WAV 저장 bit depth |
| `WAV_BYTES_PER_FRAME` | `4` | `2 channel * 16 bit / 8` |
| `INTERNAL_BYTES_PER_FRAME` | `8` | `2 channel * sizeof(int32_t)` |
| `AUDIO_BLOCK_FRAMES` | `256` | 실시간 처리 기본 단위 |
| `AUDIO_BLOCK_DURATION_MS` | 약 `5.80 ms` | `256 / 44100 * 1000` |
| `INTERNAL_AUDIO_BLOCK_BYTES` | `2048` | `256 frame * 8 byte/frame` |
| `WAV_AUDIO_BLOCK_BYTES` | `1024` | `256 frame * 4 byte/frame` |
| `STORAGE_CHUNK_BLOCKS` | `4` | SD I/O 1회에 묶을 audio block 수 |
| `STORAGE_CHUNK_FRAMES` | `1024` | `256 frame * 4 block` |
| `STORAGE_CHUNK_BYTES` | `4096` | `1024 frame * 4 byte/frame` |
| `STORAGE_CHUNK_DURATION_MS` | 약 `23.22 ms` | `1024 / 44100 * 1000` |

# 7. FX 및 믹싱과의 관계

FX와 트랙 믹싱이 오디오 포맷에 어떤 요구사항을 갖는지 정리한다.

| 항목 | 결정 |
| --- | --- |
| FX 입력 포맷 | `int32_t` interleaved stereo audio block을 입력으로 받는다. 1회 처리 단위는 `256 frame`이며 sample 순서는 `L, R, L, R, ...`이다. sample 값은 16-bit PCM 기준값을 `int32_t`로 확장한 범위를 기본 입력 범위로 둔다. |
| FX 출력 포맷 | 입력과 동일하게 `int32_t` interleaved stereo audio block을 출력한다. FX 내부에서 값이 일시적으로 16-bit 범위를 넘어갈 수 있으나, 다음 처리 단계로 전달되는 buffer layout은 변경하지 않는다. |
| 믹싱 누적 타입 | 트랙별 `int32_t` sample을 channel별 `int32_t` accumulator에 합산한다. 합산 순서는 frame 단위로 `L`, `R`을 각각 독립적으로 처리한다. |
| 여러 트랙 합산 시 gain 정책 | 트랙별 TFX와 track gain을 먼저 적용한 뒤 활성 트랙을 합산한다. 활성 트랙 수가 `N`개일 때 기본 믹싱 결과는 `sum / N`으로 정규화한다. 믹싱 이후에는 master gain만 적용한다. |
| 최종 출력 변환 | 최종 출력 직전에 `int32_t` sample을 `int16_t` 범위로 saturation한다. WAV 저장 시에는 16-bit little-endian PCM으로 기록하고, SAI 출력 시에는 DAC용 24-bit I2S slot에 맞춰 확장한다. |

# 8. 포맷 선택 타당성 및 제약

현재 선택한 저장 포맷이 프로젝트 목적에 적합한지 검토하고, 이 포맷에서 발생하는 제약을 정리한다.

| 항목 | 결정 |
| --- | --- |
| 현재 포맷 판단 | `44.1 kHz`, `16-bit`, `stereo`, WAV PCM 조합은 현재 프로젝트의 오디오 품질, SD 저장량, 처리 비용, 구현 단순성 사이에서 타당한 기본값이다. |
| WAV PCM 선택 | 압축을 사용하지 않는 linear PCM WAV로 저장한다. 파일 header가 단순하고, sample을 시간 순서대로 append할 수 있어 FatFs 기반 녹음/재생 구현이 쉽다. |
| 44.1 kHz 선택 | 듣기 좋은 음질을 확보하기 위한 최소 기준으로 둔다. 현재 `.ioc`에서 `SAI1.AudioFrequency=SAI_AUDIO_FREQUENCY_44K`, `SAI1.RealAudioFreq=44.1 KHz`, `ErrorAudioFreq=0.0%`로 설정되어 있어 SAI clock 설정과도 일치한다. |
| 16-bit 선택 | SD 카드 저장량과 처리 비용을 줄이기 위한 기본 저장 단위로 둔다. 24-bit 또는 32-bit보다 저장 용량과 read/write 대역폭 부담이 낮고, FX/믹싱은 내부 `int32_t`에서 처리한 뒤 저장 직전에 16-bit로 변환한다. |
| stereo 선택 | PCM5102A가 stereo 출력을 지원하고, 향후 PAN FX 또는 좌우 배치 처리를 구현할 때 channel 정보를 유지해야 하므로 저장 포맷도 stereo로 둔다. |
| 16-bit 저장과 24-bit SAI 입출력 차이 | 저장 포맷은 16-bit이지만 SAI 출력은 24-bit slot, SAI 입력은 32-bit slot을 사용한다. 내부 `int32_t` sample을 기준으로 처리하고 저장/출력 직전에 변환한다. |
| stereo 저장과 mono 마이크 입력 차이 | INMP441 단일 마이크 입력은 L/R에 동일 sample을 복제해 interleaved stereo로 저장한다. |
| 최대 파일 크기 | 특정 외부 장비와의 파일 호환 제한을 두지 않으면 표준 RIFF/WAV의 32-bit chunk size 한계 때문에 단일 WAV 파일은 약 `4 GB`까지 사용할 수 있다. 5트랙 구성에서는 현재 `16 GB` SD 카드에 같은 길이의 트랙 파일 5개를 저장한다고 가정하므로, 카드 용량 기준 트랙당 최대 파일 크기는 약 `3.2 GB`이다. |
| 최대 녹음 시간 | 16-bit stereo 44.1 kHz의 byte rate는 트랙당 `176,400 byte/s`이다. 단일 WAV 파일 1개만 고려하면 약 `4 GB` 기준 `6시간 45분 47초` 녹음 가능하다. 5트랙 구성에서는 전체 write/read 대역폭이 `882,000 byte/s`가 되며, 현재 `16 GB` SD 카드 기준 약 `5시간 2분 20초`를 저장할 수 있다. |

# 9. 결정 사항 요약

최종 결정된 오디오 포맷을 한 곳에 요약한다.

| 항목 | 결정값 |
| --- | --- |
| 저장 파일 포맷 | WAV, PCM linear |
| 저장 sample rate | 44.1 kHz |
| 저장 bit depth | 16-bit |
| 저장 channel count | stereo |
| 내부 sample type | `int32_t` |
| SAI input format | I2S standard, stereo, 44.1 kHz, 32-bit slot |
| SAI output format | I2S standard, stereo, 44.1 kHz, 24-bit slot |
| buffer block size | `256 frame` audio block, SD I/O는 `1024 frame / 4096 byte` storage chunk |

# 10. todo: Bit depth 확장 계획 

현재 저장 포맷은 `44.1 kHz`, `16-bit`, `stereo` WAV로 결정한다.

향후 저장 bit depth를 24-bit 또는 32-bit로 늘릴 경우 다음 항목을 다시 산정해야 한다.

| 항목 | 수정 필요 내용 |
| --- | --- |
| WAV `fmt ` chunk | `BitsPerSample`, `BlockAlign`, `ByteRate` 값을 새 bit depth 기준으로 다시 계산한다. |
| WAV sample packing | 24-bit는 3 byte little-endian 저장, 32-bit는 4 byte little-endian 저장 여부를 결정한다. |
| `WAV_BYTES_PER_FRAME` | `channel count * bit depth / 8` 기준으로 다시 계산한다. |
| audio block byte 수 | `AUDIO_BLOCK_FRAMES`를 유지할 경우 block당 byte 수가 증가한다. RAM 사용량을 다시 확인한다. |
| SD write/read chunk | `4096 byte` chunk를 유지할지, `1024 frame` chunk를 유지할지 다시 결정한다. |
| SD 대역폭 | 16-bit stereo 대비 24-bit는 1.5배, 32-bit는 2배의 read/write 대역폭이 필요하다. |
| 최대 녹음 시간 | 같은 SD 카드 용량에서 bit depth 증가 비율만큼 최대 녹음 시간이 감소한다. |
| 최종 saturation 기준 | WAV 저장 직전의 saturation 범위를 새 bit depth의 signed PCM 범위로 변경한다. |
| 믹싱 accumulator | 32-bit full-scale sample을 여러 트랙 합산할 경우 `int64_t` accumulator 사용 여부를 검토한다. |
| 외부 파일 호환성 | 현재 프로젝트는 특정 상용 루프스테이션과의 import/export 호환을 목표로 하지 않으므로, bit depth 변경 시 자체 파일 포맷 정책과 변환 도구 필요 여부를 별도로 결정한다. |
