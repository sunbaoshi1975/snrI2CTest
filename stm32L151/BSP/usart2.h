
#ifndef __USART2_H
#define __USART2_H

// used for debug log
#include "stm32l1xx.h"

#define SerialPutString(x) Serial_PutString((uint8_t*)(x))

/* Exported functions ------------------------------------------------------- */
uint32_t SerialKeyPressed(uint8_t *key);
uint8_t GetKey(void);
void SerialPutChar(uint8_t c);
void Serial_PutString(uint8_t *s);
void Serial_PutData(const uint8_t *TxBuffer, uint16_t len);

void Serial_Usart2_Init(const uint32_t speed, const uint8_t enableRxInt);

#endif  /* __USART2_H */
