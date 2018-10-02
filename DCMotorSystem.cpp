#include "DCMotorSystem.h"

void InitPWMMotors()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; /* Use the alternative pin functions */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; /* GPIO speed - has nothing to do with the timer timing */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; /* Push-pull */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; /* Setup pull-down resistors */
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_6);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_6);
	
	PWMInitDC(200);
}

int PWMInitDC(int pwm_freq)
{
    /* Calculates the timing. This is common for all channels */
    //int clk = 72e6; /* 72MHz -> system core clock. This is default on the stm32f3 discovery */
    int clk = 50e6; /* (APB1 max) 36MHz. This is default on the stm32f3 discovery */
    int tim_freq = 4e6; /* in Hz (2MHz) Base frequency of the pwm timer */
    int prescaler = ((clk / tim_freq) - 1);
 
    // Calculate the period for a given pwm frequency
    //      int pwm_freq = 200; // in Hz
    int pwm_period = tim_freq / pwm_freq;       /* 2MHz / 200Hz = 10000 */
                                                /* For 50Hz we get: 2MHz / 50Hz = 40000 */
 
    // Calculate a number of pulses per millisecond.
    // Not used in this rutine but I put it here just as an example
    int ms_pulses = (float)pwm_period / (1000.0 / pwm_freq); /* for 200Hz we get: 10000 / (1/200 * 1000) = 2000 */
 
 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
 
    /* Setup the timing and configure the TIM1 timer */
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
    TIM_TimeBaseStructure.TIM_Period = pwm_period - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; /* = 0 */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up ;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
 
 
    /* Initialise the timer channels */
    TIM_OCInitTypeDef  TIM_OCInitStructure;
 
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = ms_pulses * 2; /* preset pulse width 0..pwm_period */
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; /* Pulse polarity */
 
    /* Setup channels */
    /* Channel 1 */
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
 
    /* Channel 2 */
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
 
    /* Starup the timer */
    TIM_Cmd(TIM1, ENABLE);
 
    return pwm_period;
}

void vMotorControlSetPwmValues(int32_t pwmA, int32_t pwmB)
{
    // TIM4->CCR1 = [0..pwm_period] -> 0..100% duty cycle
    int pwmFactor = 100;
 
    TIM1->CCR1 = pwmA * pwmFactor;
    TIM1->CCR2 = pwmB * pwmFactor;
}