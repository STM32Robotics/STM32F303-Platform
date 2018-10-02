#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include "stm32f30x.h"

//Global delay variables
extern volatile uint32_t TimeDelayMS;
extern volatile uint32_t TimeDelayUS;
extern volatile bool StartDelay;

//Prototypes
extern "C" { void SysTick_Handler(); }
void SysTickInit();

#endif