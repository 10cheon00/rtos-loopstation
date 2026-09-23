#ifndef AUDIO_DMA_EVENT_HPP
#define AUDIO_DMA_EVENT_HPP

#include <cstdint>

enum class AudioDmaEvent : uint8_t {
  ERROR = 0,
  SAI_RX_HALF_COMPLETE,
  SAI_TX_HALF_COMPLETE,
  SAI_RX_COMPLETE,
  SAI_TX_COMPLETE,
};

#endif
