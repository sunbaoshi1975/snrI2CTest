#ifndef __GLOBAL_H
#define __GLOBAL_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MULTI_COLLECT_INTERVAL_10MS			50   			// 50 * 10ms = 500ms

// MULTI_COLLECT FlagBitmap
#define MULTI_COLLECT_FLAG_MIC		        0x10			// Read MIC

// Put global variables here
extern volatile uint8_t gCollectFlag;

#ifdef __cplusplus
}
#endif

#endif /* __GLOBAL_H */
