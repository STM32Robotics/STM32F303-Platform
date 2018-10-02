#include "SysTick.h"

volatile uint32_t TimeDelayMS = 0;
volatile uint32_t TimeDelayUS = 0;
volatile bool StartDelay = false;

extern "C" 
{
	void SysTick_Handler()
	{
		if(StartDelay)
		{
			TimeDelayUS += 10; //Count up by micro seconds and convert to milli
			if(TimeDelayUS % 1000 == 0)
				TimeDelayMS++;
		}
	}
}

void SysTickInit()
{
	uint32_t ticks = 4 * 16 * 1000000; //Clock rate 16 PLL 4MHz
	ticks /= 100000;
	SysTick->CTRL = 0;
	SysTick->LOAD = ticks -1 ;
	NVIC_SetPriority(SysTick_IRQn, 0); //Enable SysTick_Handler
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	StartDelay = false; //Initialize variables
	TimeDelayMS = TimeDelayUS = 0;
}