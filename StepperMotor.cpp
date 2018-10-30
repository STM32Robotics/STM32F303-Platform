#include "StepperMotor.h"
#include "stm32f30x.h"
#include "Utilities.h"
#include "TimerCallback.h"
#include "SysTick.h"

static GPIO_InitTypeDef pc_InitStruct, pb_InitStruct;
static TIM_TimeBaseInitTypeDef tim16_initStruct;
static NVIC_InitTypeDef nvic_initStruct;

static unsigned char init_flag = 0;
static char step_typ;
static unsigned int step_flag = 0;
static unsigned int step_cnt = 0;

void StepperInit()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	pc_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	pc_InitStruct.GPIO_OType = GPIO_OType_PP;
	pc_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	pc_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	pc_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_Init(GPIOC, &pc_InitStruct);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	pc_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	pc_InitStruct.GPIO_OType = GPIO_OType_PP;
	pc_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_10;
	pc_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	pc_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_Init(GPIOC, &pc_InitStruct);
  
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM4, ENABLE);
	
	tim16_initStruct.TIM_Prescaler = PRESCALE_STEPPER;
	tim16_initStruct.TIM_Period = PERIOD_STEPPER;
	tim16_initStruct.TIM_ClockDivision = 0;
	tim16_initStruct.TIM_RepetitionCounter = 0;
	tim16_initStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &tim16_initStruct);
	
	nvic_initStruct.NVIC_IRQChannel = TIM4_IRQn;
	nvic_initStruct.NVIC_IRQChannelCmd = ENABLE;
	nvic_initStruct.NVIC_IRQChannelPreemptionPriority = 1;
	nvic_initStruct.NVIC_IRQChannelSubPriority = 2;

	NVIC_Init(&nvic_initStruct);
	
	TIM_Cmd(TIM4, ENABLE);
	TIM_ITConfig(TIM4,TIM_IT_Update , ENABLE);
	
	TIM_UpdateDisableConfig(TIM4, ENABLE);

	AddTimerCallback("TIM4", &TIM4Hand);
}

volatile int StepperCnt = 2;
volatile bool IsInitialized = true;
volatile int step_amount = 0;
int GetStepAmount()
{
	return step_amount;
}

void StepperPosInit(void) 
{   
		step_cnt = 0;
		IsInitialized = false;
		StepperCnt = 3;
    Stepper(DIR_RIGHT, HALF_STEP, STEP_MAX, false);
		//SetLEDColor(LED_Red);
	
		StartTimerCounter();
    while(!(GPIOB->IDR & LMT_SW_RIGHT))
		{
			if(GetTimerCountMS() > 5000)
			{
				SetLEDColor(LED_Red);
				return;
			}
    }      
    //SetLEDColor(LED_Magenta);    
    //TIM_UpdateDisableConfig(TIM4, ENABLE); 
    step_flag = 0;
		//ZeroTimerCount();
		/*while(StepperCnt == 2)
		{
			if(GetTimerCountMS() > 5000)
			{
				SetLEDColor(LED_Blue);
				return;
			}
		}*/
		step_cnt = 0;
		ZeroTimerCount();
    Stepper(DIR_LEFT, HALF_STEP, STEP_MAX, false);
    while(!(GPIOB->IDR & LMT_SW_LEFT))
		{
			if(GetTimerCountMS() > 5000)
			{
				SetLEDColor(LED_White);
				return;
			}
    }
    step_flag = 0;
    //TIM_UpdateDisableConfig(TIM4, ENABLE); 
		/*ZeroTimerCount();
		while(StepperCnt == 1)
		{
			if(GetTimerCountMS() > 5000)
			{
				SetLEDColor(LED_Yellow);
				return;
			}
		}*/
    if (step_cnt % 2)
        step_cnt++;

    step_flag = step_cnt / 2;
		step_amount = step_cnt / 2;
		ZeroTimerCount();
    Stepper(DIR_RIGHT, HALF_STEP, step_flag, false);
		while(StepperCnt == 1)
		{
			if(GetTimerCountMS() > 5000)
			{
				SetLEDColor(LED_Orange);
				return;
			}
		}
		IsInitialized = true;
		EndTimerCounter();
}

int givenDir = DIR_LEFT;
void Stepper(unsigned dir, unsigned char step_size, unsigned int step_count, bool IsUser) 
{   
	Delay(1); //Needed
	if(IsUser && !IsInitialized)
	{
		return;
	}
	if((GPIOB->IDR & LMT_SW_RIGHT) && dir == DIR_RIGHT)
	{
		TIM_UpdateDisableConfig(TIM4, ENABLE); 
		return;
	}
	if((GPIOB->IDR & LMT_SW_LEFT) && dir == DIR_LEFT)
	{
		TIM_UpdateDisableConfig(TIM4, ENABLE); 
		return;
	}
	givenDir = dir;
	step_typ = (char)step_size;

	if(dir)
			step_typ *= DIR_LEFT;
	
	step_flag = step_count;
 
	TIM_UpdateDisableConfig(TIM4, DISABLE);    
}

uint16_t stepper_lut[] = {0x200, 0x280, 0x080, 0x180, 0x100, 0x140, 0x040, 0x240};
static char pos = 0;

void TIM4Hand()
	{				
			if (step_flag) {
				if((GPIOB->IDR & LMT_SW_RIGHT) && givenDir == DIR_RIGHT)
				{
					step_flag = 0;
					TIM_UpdateDisableConfig(TIM4, ENABLE); 
					return;
				}
				if((GPIOB->IDR & LMT_SW_LEFT) && givenDir == DIR_LEFT)
				{
					step_flag = 0;
					TIM_UpdateDisableConfig(TIM4, ENABLE); 
					return;
				}
					FORCE_BITS(GPIOC->ODR, STEPPER_MASK, stepper_lut[pos]);
					pos += step_typ;
					pos &= 0b00000111;
					//Todo: Exhuast the queue content if limit switch has been hit
					step_flag--;
					if(!IsInitialized)
						step_cnt++;
					if(givenDir == DIR_LEFT)
						step_amount--;
					else
						step_amount++;
					
			} else {
					TIM_UpdateDisableConfig(TIM4, ENABLE);
					if(StepperCnt > 0)
					{
						StepperCnt--;
					}
			}
	}