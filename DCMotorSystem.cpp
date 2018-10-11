#include "DCMotorSystem.h"
#include "stm32f30x.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_rcc.h"
#include "SysTick.h"
#include "Utilities.h"

#define TIM1_CNT_CLOCK 2000000
#define PRESCALE  (uint32_t)((SystemCoreClock / TIM1_CNT_CLOCK) - 1)
#define PERIOD 99

static GPIO_InitTypeDef pa_InitStruct, pb_InitStruct;
static TIM_TimeBaseInitTypeDef tim1_initStruct;
static TIM_OCInitTypeDef t1oc_config;

static TIM_BDTRInitTypeDef bdtr_initStruct;

void DCMotorInit(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    /* Initialize GPIO Signal for controlling DC Motor Direction*/
    pb_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    pb_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    pb_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    pb_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;    
    pb_InitStruct.GPIO_OType = GPIO_OType_PP;
    
    GPIO_Init(GPIOB, &pb_InitStruct);

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    /* Initialize GPIO Signal for controlling DC Motor Direction*/
    pa_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    pa_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    pa_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    pa_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;    
    pa_InitStruct.GPIO_OType = GPIO_OType_PP;
    
    GPIO_Init(GPIOA, &pa_InitStruct);
    
    /* Initialize PWM signal for controlling DC Motor Speed */
    pa_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    pa_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;  

    GPIO_Init(GPIOA, &pa_InitStruct);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8,  GPIO_AF_6);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,  GPIO_AF_6);

    RCC_APB2PeriphClockCmd (RCC_APB2Periph_TIM1, ENABLE);
    
    tim1_initStruct.TIM_Prescaler = PRESCALE;
    tim1_initStruct.TIM_Period = PERIOD;
    tim1_initStruct.TIM_ClockDivision = 0;
    tim1_initStruct.TIM_RepetitionCounter = 0;
    tim1_initStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &tim1_initStruct);
    
    /* OC Generic */
    t1oc_config.TIM_OCMode = TIM_OCMode_PWM1;
    t1oc_config.TIM_OCPolarity = TIM_OCPolarity_High;
    
    /* PWM SPECIFIC */
    t1oc_config.TIM_OCIdleState = TIM_OCIdleState_Reset;
    t1oc_config.TIM_Pulse = (uint32_t)(PERIOD / 2);
    t1oc_config.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC1Init(TIM1, &t1oc_config);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    
    TIM_OC2Init(TIM1, &t1oc_config);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM1, ENABLE);
    
    /*Advance TIM SPECIFICS */
    TIM_BDTRStructInit(&bdtr_initStruct);
    TIM_BDTRConfig(TIM1, &bdtr_initStruct);
    TIM_CCPreloadControl(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    TIM_Cmd(TIM1, ENABLE);
}

void DCMotor(motor_sel which_motor, unsigned char const speed, motor_dir direction)
{
    unsigned char temp_speed = speed;
    
    if(temp_speed > MAXSPEED)
        temp_speed = MAXSPEED;
    else
        temp_speed = speed;
		
		double slope1 = 52.0;
		double slope2 = 100.0;
		double x = (double)temp_speed;
		double b = 50.0;
		
		temp_speed = (unsigned char)(slope1 / slope2 * x + b);
    
    if(which_motor == M_ONE) {
            CLR_BITS(GPIOA->ODR, DCMOTOR1_MASK);
    
        if (direction == FORWARD)
            SET_BITS(GPIOA->ODR, DC_M1_FW);
        else
            SET_BITS(GPIOA->ODR, DC_M1_BW);
        TIM1->CCR2 = temp_speed;
        
    } else {
        CLR_BITS(GPIOB->ODR, DCMOTOR2_MASK);

         if (direction == FORWARD)      
            SET_BITS(GPIOB->ODR, DC_M2_FW);
        else
            SET_BITS(GPIOB->ODR, DC_M2_BW);
				TIM1->CCR1 = temp_speed;
    }
}