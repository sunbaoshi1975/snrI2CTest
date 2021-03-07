#include "timer.h"
Tim_CallBack_t Timer_1ms_handler = NULL;
Tim_CallBack_t Timer_5ms_handler = NULL;
Tim_CallBack_t Timer_10ms_handler = NULL;

uint8_t TimCount = 10;
void Timer_Init(void) {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	// 1ms interval
	TIM_TimeBaseStructure.TIM_Period = TIMER_PERIOD;
	TIM_TimeBaseStructure.TIM_Prescaler = TIMER_PRESCALER;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	//TimCount = 10;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET) {
		if(Timer_1ms_handler) (*Timer_1ms_handler)();
		if(TimCount > 0) {
			TimCount--;
			if( TimCount == 0 ) {
				TimCount = 10;
				if(Timer_10ms_handler) (*Timer_10ms_handler)();
			} else if(TimCount %5 == 2) {
				if(Timer_5ms_handler) (*Timer_5ms_handler)();
			}
		}
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}
