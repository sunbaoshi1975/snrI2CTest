#ifndef __SEN_MIC_H
#define __SEN_MIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nsr_i2c.h"

// Noise level (dB)
extern uint16_t gMIC_noiseValue;

uint8_t mic_init(void);
uint8_t mic_IsDataReady(void);
uint8_t mic_checkData(void);

#ifdef __cplusplus
}
#endif

#endif /* __SEN_MIC_H */
