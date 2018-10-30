#include "ServoSystem.h"
#include "stm32f30x_tim.h"
#include "TimerCallback.h"
#include "LCDSystem.h"
/******************************************************************************
    
*******************************************************************************/
#define TIM2_CNT_CLOCK 500000
#define PRESCALE  (uint32_t)((SystemCoreClock / TIM2_CNT_CLOCK) - 1)
#define PERIOD 9999
#define PWM_START_PULSE (uint32_t)(PERIOD / 2)

static GPIO_InitTypeDef pb_InitStruct;
static TIM_TimeBaseInitTypeDef tim2_initStruct;
static TIM_OCInitTypeDef t2oc_config;
static NVIC_InitTypeDef nvic_initStruct;
static uint32_t _servo_deg = 0;

void ServoInit(void)
{

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

     /* Initialize PWM signal for controlling servo */
    pb_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    pb_InitStruct.GPIO_Pin = GPIO_Pin_11;  
    pb_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    pb_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;    
    pb_InitStruct.GPIO_OType = GPIO_OType_PP;

    GPIO_Init(GPIOB, &pb_InitStruct);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11,  GPIO_AF_1);

    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, ENABLE);
    
    tim2_initStruct.TIM_Prescaler = PRESCALE;
    tim2_initStruct.TIM_Period = PERIOD;
    tim2_initStruct.TIM_ClockDivision = 0;
    tim2_initStruct.TIM_RepetitionCounter = 0;
    tim2_initStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &tim2_initStruct);
		
    nvic_initStruct.NVIC_IRQChannel = TIM2_IRQn;
    nvic_initStruct.NVIC_IRQChannelCmd = ENABLE;
    nvic_initStruct.NVIC_IRQChannelPreemptionPriority = 1;
    nvic_initStruct.NVIC_IRQChannelSubPriority = 3;
		NVIC_Init(&nvic_initStruct);
		
    /* OC Generic */
    t2oc_config.TIM_OCMode = TIM_OCMode_PWM1;
    t2oc_config.TIM_OCPolarity = TIM_OCPolarity_High;
    
    /* PWM SPECIFIC */
    t2oc_config.TIM_OCIdleState = TIM_OCIdleState_Reset;
    t2oc_config.TIM_Pulse = (uint32_t)(PERIOD / 2);
    //t2oc_config.TIM_OutputState = TIM_OutputState_Enable;
		
		
		
    TIM_OC4Init(TIM2, &t2oc_config);
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

    
		TIM2->DIER |= TIM_IT_CC4;
		
		uint32_t tmp = 0;
		tmp = TIM_CCER_CC1E << 0x000CU;
		TIM2->CCER &= ~tmp;
		TIM2->CCER |= (uint32_t)(0x0001U << 0x000CU);
		
		TIM2->CR1 |= (TIM_CR1_CEN);
		
    //TIM_ARRPreloadConfig(TIM2, DISABLE);
    
    //TIM_Cmd(TIM2, ENABLE);
		//TIM_ITConfig(TIM2, TIM_IT_CC4, ENABLE);
		
		//AddTimerCallback("TIM2CH4", &Timer2PWM);
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

void Timer2PWM(void)
{
	uint32_t turn_cnt = (SERVO_CNT_GAIN * _servo_deg) + SERVO_CNT_OFFSET;
	TIM2->CCR4 = turn_cnt;
}
