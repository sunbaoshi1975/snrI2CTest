#ifndef __COMMON_H
#define __COMMON_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"

// Common Data Type
#ifndef UC
#define UC                        uint8_t
#endif

#ifndef US
#define US                        uint16_t
#endif

#ifndef UL
#define UL                        uint32_t
#endif

#ifndef SHORT
#define SHORT                     int16_t
#endif

#ifndef LONG
#define LONG                      int32_t
#endif

#ifndef BOOL
#define BOOL                      uint8_t
#endif

#ifndef TRUE
#define TRUE                    	1
#endif

#ifndef FALSE
#define FALSE                  		0
#endif

#ifndef NULL
#define NULL                        0
#endif

#define DTIT_RS_DEFAULT_BAUDRATE			9600

/* Common routines */
#define IS_AF(c)  ((c >= 'A') && (c <= 'F'))
#define IS_af(c)  ((c >= 'a') && (c <= 'f'))
#define IS_09(c)  ((c >= '0') && (c <= '9'))
#define ISVALIDHEX(c)  IS_AF(c) || IS_af(c) || IS_09(c)
#define ISVALIDDEC(c)  IS_09(c)
#define CONVERTDEC(c)  (c - '0')

#define CONVERTHEX_alpha(c)  (IS_AF(c) ? (c - 'A'+10) : (c - 'a'+10))
#define CONVERTHEX(c)   (IS_09(c) ? (c - '0') : CONVERTHEX_alpha(c))

#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24UL)
#define DAYS_PER_WEEK (7UL)
#define SECS_PER_WEEK (SECS_PER_DAY * DAYS_PER_WEEK)

#ifndef ENTER_CRITICAL_SECTION
#define ENTER_CRITICAL_SECTION( )  __disable_irq()
#endif

#ifndef EXIT_CRITICAL_SECTION
#define EXIT_CRITICAL_SECTION( )   __enable_irq()
#endif

/* Exported functions ------------------------------------------------------- */
void clearBuffer(uint8_t *_buf, const uint16_t _len);
void copyBuffer(uint8_t *_toBuf, const uint8_t *_fromBuf, const uint16_t _len);
void copySwapByteBuf(uint8_t *_toBuf, const uint8_t *_fromBuf, const uint16_t _len);

uint16_t CRC16_Check(const uint8_t *pushMsg, uint8_t usDataLen);
uint8_t CRC8_Check(const uint8_t *pushMsg, const uint8_t ucDataLen);
uint16_t CheckSum16(const uint8_t *pushMsg, const uint16_t ucDataLen);
uint8_t CheckSum8(const uint8_t *pushMsg, const uint8_t ucDataLen);

uint32_t GetBaudRateValue(const uint8_t code);
uint32_t GetValidBaudRate(const uint8_t code);
void LogOutput(const char *format, ...);

#endif  /* __COMMON_H */
