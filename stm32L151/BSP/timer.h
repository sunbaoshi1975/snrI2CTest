#ifndef __TIMER_H
#define __TIMER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"
#include "global.h"

//#define TIMEID  TIM2
// 1ms interval
/// HSI clock: 8M
//#define TIMER_PERIOD    	999 
//#define TIMER_PRESCALER 	7
#define TIMER_PERIOD    	999
#define TIMER_PRESCALER 	15

typedef void (*Tim_CallBack_t)();
extern Tim_CallBack_t Timer_1ms_handler;
extern Tim_CallBack_t Timer_10ms_handler;
extern Tim_CallBack_t Timer_5ms_handler;

void Timer_Init(void);
#endif  /* __TIMER_H */
