#ifndef __DELAY_H
#define __DELAY_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"

void delay_init(void);
void delay_us(const uint32_t nus);
void delay_ms(const uint16_t nms);

#ifdef __cplusplus
}
#endif

#endif  /* __DELAY_H */
