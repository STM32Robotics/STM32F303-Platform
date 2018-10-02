#include "Utilities.h"

bool HasFlag(uint32_t mask, uint32_t flag)
{
	if((mask & flag) == flag)
		return true;
	return false;
}

void Delay(uint32_t nTime)
{
	TimeDelayMS = TimeDelayUS = 0;
	StartDelay = true;
	while(TimeDelayMS < nTime);
	StartDelay = false;
}

void DelayMicro(uint32_t nTime)
{
	if(nTime < 10)
		nTime = 10; //Minimum tick count is 10us, lower will hang
	TimeDelayMS = TimeDelayUS = 0;
	StartDelay = true;
	while(TimeDelayUS < nTime);
	StartDelay = false;
}

void LEDInit()
{
	GPIO_InitTypeDef gpioa_init_struct = {GPIO_Pin_5, GPIO_Mode_OUT, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL};
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_Init(GPIOA, &gpioa_init_struct);
}

void SetLEDState(bool ON)
{
	if(ON)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	}
}

void LEDToggle()
{
	LEDInit();
	if(GetLEDState())
	{
		SetLEDState(false);
		return;
	}
	SetLEDState(true);
}

bool GetLEDState()
{
	uint8_t led_bit = GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_5);
	if(led_bit == (uint8_t)Bit_SET)
		return true;
	return false;
}

bool IsStrEqual(char* str1, const char* str2)
{
	if(strlen(str1) != strlen(str2))
		return false;
	int i;
	for(i = 0; i < strlen(str2); i++)
	{
		if(str1[i] != str2[i])
			return false;
	}
	return true;
}

bool IsStrEqual(const char* str1, const char* str2)
{
	if(strlen(str1) != strlen(str2))
		return false;
	int i;
	for(i = 0; i < strlen(str2); i++)
	{
		if(str1[i] != str2[i])
			return false;
	}
	return true;
}

uint32_t PWM_Freq = 1000;
uint32_t PWM_Step = 100;

uint32_t GetClockFreq()
{
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	uint32_t mult;
	if(RCC_Clocks.PCLK1_Frequency == RCC_Clocks.SYSCLK_Frequency)
	{
		mult = 1;
	}
	else
	{
		mult = 2;
	}
	
	return mult * RCC_Clocks.PCLK1_Frequency;
}

void RGBLEDInit()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource14, GPIO_AF_5);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource13, GPIO_AF_10);

	uint32_t TIMER_Frequency = GetClockFreq();
  uint32_t COUNTER_Frequency = PWM_Step * PWM_Freq;
  uint32_t PSC_Value = (TIMER_Frequency / COUNTER_Frequency) - 1;
  uint16_t ARR_Value = PWM_Step - 1;
 
  RCC_APB2PeriphClockCmd (RCC_APB2Periph_TIM8, ENABLE);
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_TimeBaseStructInit (&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = ARR_Value;
  TIM_TimeBaseStructure.TIM_Prescaler = PSC_Value;
  TIM_TimeBaseInit (TIM8, &TIM_TimeBaseStructure);
 
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  TIM_OCStructInit (&TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OC1Init(TIM8, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
  TIM_ARRPreloadConfig(TIM8, ENABLE);
	
	TIM_BDTRInitTypeDef TIM_BDTRInitStruct;
	TIM_BDTRStructInit(&TIM_BDTRInitStruct);
	TIM_BDTRConfig(TIM8, &TIM_BDTRInitStruct);
	TIM_CCPreloadControl(TIM8, ENABLE);
	TIM_CtrlPWMOutputs(TIM8, ENABLE);

  TIM_Cmd(TIM8, ENABLE);
	
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM4, ENABLE);
  TIM_TimeBaseStructInit (&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = ARR_Value;
  TIM_TimeBaseStructure.TIM_Prescaler = PSC_Value;
  TIM_TimeBaseInit (TIM4, &TIM_TimeBaseStructure);
 
  TIM_OCStructInit (&TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
  TIM_ARRPreloadConfig(TIM4, ENABLE);
	
	TIM_Cmd(TIM4, ENABLE);
}

void SetLEDColor(int r, int g, int b)
{
	RGBLEDInit();
	TIM_SetCompare1(TIM8, r); //red
	TIM_SetCompare2(TIM8, g); //green
	TIM_SetCompare3(TIM4, b); //blue
}
