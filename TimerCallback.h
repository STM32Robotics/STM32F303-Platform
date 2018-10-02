#ifndef TIMERC_H_
#define TIMERC_H_

#include "stm32f30x.h"
#include "Utilities.h"

#define TimerSize 32

void InitTimerCallback();
//Timer = "TIM1CH2"
void AddTimerCallback(const char* timer, void(*func)());
void ExecuteTimerCallback(const char* timer);

extern "C"
{
	void TIM1_CC_IRQHandler();
	void TIM2_IRQHandler();
	void TIM3_IRQHandler();
	void TIM4_IRQHandler();
	void TIM7_IRQHandler();
	void TIM8_CC_IRQHandler();
	void TIM20_CC_IRQHandler();
};

#endif