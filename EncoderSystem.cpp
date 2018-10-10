#include "EncoderSystem.h"
#include "stm32f30x_rcc.h"
#include "SysTick.h"
#include "stm32f30x_tim.h"
#include "TimerCallback.h"

static GPIO_InitTypeDef pa_InitStruct;
static TIM_TimeBaseInitTypeDef tim3_initStruct;
static TIM_ICInitTypeDef t3it_initStruct;
static NVIC_InitTypeDef nvic_initStruct;

#define TIM3_CNT_CLOCK 1000000
#define PRESCALE  (uint32_t)((SystemCoreClock / TIM3_CNT_CLOCK) - 1)
#define PERIOD 0x0000FFFF
#define VANE_NUM 27.0
#define GEAR_RATIO 22.5
#define WHEEL_ROTATION 16.65
#define NUM_US_IN_SEC 1000000.0

typedef struct
{
    uint32_t pwm;
    uint32_t period;
    uint32_t curr;
}ENC_WAVE;

static uint32_t period = 0;
static uint32_t last_cap = 0;
static uint8_t counter = 0;
void EncoderInit(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    
    pa_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    pa_InitStruct.GPIO_Pin = GPIO_Pin_6;
    pa_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    pa_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
    //pa_InitStruct.GPIO_OType = GPIO_OType_PP;
    
    GPIO_Init(GPIOA, &pa_InitStruct);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_2);
    
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM3, ENABLE);
  
    tim3_initStruct.TIM_Prescaler = PRESCALE;
    tim3_initStruct.TIM_Period = PERIOD;
    tim3_initStruct.TIM_ClockDivision = 0;
    tim3_initStruct.TIM_RepetitionCounter = 0;
    tim3_initStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &tim3_initStruct);
    
    t3it_initStruct.TIM_Channel = TIM_Channel_1;
    t3it_initStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
    t3it_initStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
    t3it_initStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    t3it_initStruct.TIM_ICFilter = 0x00;
    TIM_ICInit(TIM3, &t3it_initStruct);
    
    nvic_initStruct.NVIC_IRQChannel = TIM3_IRQn;
    nvic_initStruct.NVIC_IRQChannelCmd = ENABLE;
    nvic_initStruct.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_initStruct.NVIC_IRQChannelSubPriority = 0;
    
    NVIC_Init(&nvic_initStruct);
  
    TIM_Cmd(TIM3, ENABLE);
    
    TIM_ITConfig(TIM3,TIM_IT_CC1, ENABLE);
		
		AddTimerCallback("TIM3", &Timer3Handler);
}


uint32_t GetWavePeriod()
{
    //period = period  & 0x0000FFFF;
    return period;
}

uint32_t GetSpeed()
{
   double speed, freq;
   double prd = 0.0;
    
   prd = period;
    
   freq = (1.0/prd);
   speed = (((freq * NUM_US_IN_SEC) / VANE_NUM ) / GEAR_RATIO) * WHEEL_ROTATION;

   return (uint32_t)speed;
}

void Timer3Handler()
{
		volatile uint32_t curr_cap;
		
		if( (TIM3->SR & TIM_SR_CC1IF) != 0) {
				curr_cap = TIM3->CCR1;
				
				period = curr_cap - last_cap;

				last_cap = curr_cap;
		 
				if ( (TIM3->SR  & TIM_SR_UIF) != 0)
						TIM3->SR &= ~TIM_SR_UIF;

		}
}