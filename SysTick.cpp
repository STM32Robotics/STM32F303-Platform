#include "SysTick.h"

volatile uint32_t TimeDelayMS = 0;
volatile uint32_t TimeDelayUS = 0;
volatile bool StartDelay = false;
volatile uint32_t TimerCount = 0;
volatile bool StartTimer = false;
volatile uint32_t TimerCountUS = 0;

extern "C" //Needed for C++ compiler to keep function name at runtime visible for STM
{
	void SysTick_Handler()
	{
		if(StartDelay)
		{
			TimeDelayUS += 10; //Count up by micro seconds and convert to milli
			if(TimeDelayUS % 1000 == 0)
				TimeDelayMS++;
		}
		if(StartTimer)
		{
			TimerCountUS += 10;
			if(TimerCountUS % 1000 == 0)
				TimerCount++;
		}
	}
}

void SysTickInit()
{
	uint32_t ticks = 4 * 16 * 1000000; //Clock rate 16 PLL 4MHz
	ticks /= 100000;
	SysTick->CTRL = 0;
	SysTick->LOAD = ticks -1 ;
	
	NVIC_InitTypeDef sys;
	sys.NVIC_IRQChannel = SysTick_IRQn;
	sys.NVIC_IRQChannelPreemptionPriority = 0;
	sys.NVIC_IRQChannelSubPriority = 0;
	sys.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&sys);
	//NVIC_SetPriority(SysTick_IRQn, 0); //Enable SysTick_Handler
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	StartDelay = false; //Initialize variables
	TimeDelayMS = TimeDelayUS = 0;
}

void StartTimerCounter()
{
	TimerCount = 0;
	TimerCountUS = 0;
	StartTimer = true;
}

void EndTimerCounter()
{
	StartTimer = false;
}

int GetTimerCountMS()
{
	return TimerCount;
}

int GetTimerCountUS()
{
	return TimerCountUS;
}

void ZeroTimerCount()
{
	TimerCount = 0;
	TimerCountUS = 0;
}