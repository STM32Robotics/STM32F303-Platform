#include "StepperMotor.h"
#include "stm32f30x.h"
#include "Utilities.h"
#include "TimerCallback.h"

#define TIM4_CNT_CLOCK 200000
#define PRESCALE  (uint32_t)((SystemCoreClock / TIM4_CNT_CLOCK) - 1)
#define PERIOD 350

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
    
    tim16_initStruct.TIM_Prescaler = PRESCALE;
    tim16_initStruct.TIM_Period = PERIOD;
    tim16_initStruct.TIM_ClockDivision = 0;
    tim16_initStruct.TIM_RepetitionCounter = 0;
    tim16_initStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &tim16_initStruct);
    
    nvic_initStruct.NVIC_IRQChannel = TIM4_IRQn;
    nvic_initStruct.NVIC_IRQChannelCmd = ENABLE;
    nvic_initStruct.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_initStruct.NVIC_IRQChannelSubPriority = 0;
    
    NVIC_Init(&nvic_initStruct);
    TIM_Cmd(TIM4, ENABLE);
    TIM_ITConfig(TIM4,TIM_IT_Update, ENABLE);
    TIM_UpdateDisableConfig(TIM4, ENABLE); 
    
		AddTimerCallback("TIM4", &Timer4Handler);
}

void StepperPosInit(void) 
{   
    Stepper(DIR_RIGHT, HALF_STEP, STEP_MAX);
    while(!(GPIOB->IDR & LMT_SW_RIGHT)){
    }      
        
    TIM_UpdateDisableConfig(TIM4, ENABLE); 
    
    step_cnt = 0;    
    Stepper(DIR_LEFT, HALF_STEP, STEP_MAX);
    
    while(!(GPIOB->IDR & LMT_SW_LEFT)){
    }
    
    TIM_UpdateDisableConfig(TIM4, ENABLE); 

    if (step_cnt % 2)
        step_cnt++;

    step_flag = step_cnt / 2;
    Stepper(DIR_RIGHT, HALF_STEP, step_flag);                             
}

void Stepper(unsigned dir, unsigned char step_size, unsigned int step_count) 
{   
    step_typ = (char)step_size;

    if(dir)
        step_typ *= DIR_LEFT;
    
    step_flag = step_count;
   
    TIM_UpdateDisableConfig(TIM4, DISABLE);    
}

void Timer4Handler()
{
		uint16_t stepper_lut[] = {0x200, 0x280, 0x080, 0x180, 0x100, 0x140, 0x040, 0x240};
		static unsigned char pos = 0;
								
		if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) {
				TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

				//Todo: instead of using step_flag, use a step command queue, check if queue is empty
				if (step_flag) {
						FORCE_BITS(GPIOC->ODR, STEPPER_MASK, stepper_lut[pos]);
						pos += step_typ;
						pos &= 0b00000111;
						//Todo: Exhuast the queue content if limit switch has been hit
						step_flag--;
						step_cnt++;
				} else {
						TIM_UpdateDisableConfig(TIM4, ENABLE);
				}
		}
}