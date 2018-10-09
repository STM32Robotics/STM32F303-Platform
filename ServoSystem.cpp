#include "ServoSystem.h"
#include "stm32f30x_tim.h"
#include "TimerCallback.h"

#define TIM2_CNT_CLOCK 500000
#define PRESCALE  (uint32_t)((SystemCoreClock / TIM2_CNT_CLOCK) - 1)
#define PERIOD 9999
#define PWM_START_PULSE (uint32_t)(PERIOD / 2)

static TIM_OCInitTypeDef t2oc_config;
static TIM_TimeBaseInitTypeDef h_tim2;
static GPIO_InitTypeDef pb_InitStruct;
static NVIC_InitTypeDef nvic_initStruct;

static uint32_t _servo_deg = 0;

/******************************************************************************
    
*******************************************************************************/
int16_t ServoInit()
{
    int16_t rc = SRVO_INIT_SUCCESS;

		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    /* Set GPIO Alternate Function */
    pb_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
    pb_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    pb_InitStruct.GPIO_Speed = GPIO_Speed_Level_2;
    pb_InitStruct.GPIO_Pin = GPIO_Pin_11;

		GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_1);

    GPIO_Init(GPIOB, &pb_InitStruct);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    /* Timer Init */
    h_tim2.TIM_Prescaler         = PRESCALE;
    h_tim2.TIM_Period            = PERIOD;
    h_tim2.TIM_ClockDivision     = 0;
    h_tim2.TIM_CounterMode       = TIM_CounterMode_Up;
    h_tim2.TIM_RepetitionCounter = 0;

/* tim1_initStruct.TIM_Prescaler = PRESCALE;
    tim1_initStruct.TIM_Period = PERIOD;
    tim1_initStruct.TIM_ClockDivision = 0;
    tim1_initStruct.TIM_RepetitionCounter = 0;
    tim1_initStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &tim1_initStruct);
    */

    TIM_TimeBaseInit(TIM2, &h_tim2);

    /* Enable IRQ for Timer's Interrupt Mode */
		nvic_initStruct.NVIC_IRQChannel = TIM2_IRQn;
    nvic_initStruct.NVIC_IRQChannelCmd = ENABLE;
    nvic_initStruct.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_initStruct.NVIC_IRQChannelSubPriority = 0;
    
		NVIC_Init(&nvic_initStruct);
  
    TIM_Cmd(TIM2, ENABLE);
    
    TIM_ITConfig(TIM2, TIM_IT_CC4, ENABLE);
		

    t2oc_config.TIM_OCMode       = TIM_OCMode_PWM1;
    t2oc_config.TIM_OCPolarity   = TIM_OCPolarity_High;

    /* PWM SPECIFIC */

    t2oc_config.TIM_OutputState = TIM_OutputState_Enable;
    t2oc_config.TIM_OCNPolarity  = TIM_OCNPolarity_High;
    t2oc_config.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    t2oc_config.TIM_OCIdleState  = TIM_OCIdleState_Reset;
    t2oc_config.TIM_Pulse = (uint32_t)(PERIOD * 30 / 100);
		
		
		TIM_OC4Init(TIM2, &t2oc_config);
		TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM2, DISABLE);
		
		TIM_CCPreloadControl(TIM2, ENABLE);
		TIM_CtrlPWMOutputs(TIM2, ENABLE);
		
		AddTimerCallback("TIM2CH4", &PWMPulseFinish);
    
    return rc;
}

void Servo(uint32_t degree) 
{   
    /* Bounded input */
    if (degree > 180)
        degree = 180;
 
    /* Atmoic */
    __asm("ISB;CPSIE I");
    _servo_deg = degree;
    __asm("CPSIE I");
}

/******************************************************************************
- Period for Hi-Tec HS311 20ms (50Hz)
- E-Clock (Bus clock) prescaler = 16 (500K HZ)
- # of count in TCx for 20ms with 500K HZ  = 10000
- 1 count = 2 us
- 0 degree position = 600us =  300 counts
- 180 degree position = 2400us = 1200 counts
- Line Equation for degree	 to # counts mapping, y = 5x + 300.
  (x = degree, y = count)
******************************************************************************/  
void PWMPulseFinish() 
{
    uint32_t turn_cnt = (SERVO_CNT_GAIN * _servo_deg) + SERVO_CNT_OFFSET;
     TIM2->CCR4 = turn_cnt;
}
