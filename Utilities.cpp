#include "Utilities.h"

//Check if a mask has a bit flag
bool HasFlag(uint32_t mask, uint32_t flag)
{
	if((mask & flag) == flag)
		return true;
	return false;
}

//Delay in milli seconds
void Delay(uint32_t nTime)
{
	if(nTime == 0)
		return;
	TimeDelayMS = TimeDelayUS = 0; //Reset counter so it doesn't overflow
	StartDelay = true;
	while(TimeDelayMS < nTime);
	StartDelay = false;
}

//Delay in micro seconds
void DelayMicro(uint32_t nTime)
{
	if(nTime < 10)
		nTime = 10; //Minimum tick count is 10us, lower will hang
	TimeDelayMS = TimeDelayUS = 0;
	StartDelay = true;
	while(TimeDelayUS < nTime);
	StartDelay = false;
}

//Initialize LED on Nucleo
void LEDInit()
{
	GPIO_InitTypeDef gpioa_init_struct = {GPIO_Pin_5, GPIO_Mode_OUT, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL};
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_Init(GPIOA, &gpioa_init_struct);
}

//Set Nucleo LED state
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

//Toggle Nucleo LED state
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

//Get Nucleo LED state
bool GetLEDState()
{
	uint8_t led_bit = GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_5);
	if(led_bit == (uint8_t)Bit_SET)
		return true;
	return false;
}

//String comparison
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

//Get Nucleo clock frequency
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

//RGB LED Initialization
void RGBLEDInit()
{
	uint32_t PWM_Freq = 1000;
	uint32_t PWM_Step = 100;
	
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource14, GPIO_AF_5);
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_10);
	
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
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);
  TIM_ARRPreloadConfig(TIM8, ENABLE);
	
	TIM_BDTRInitTypeDef TIM_BDTRInitStruct;
	TIM_BDTRStructInit(&TIM_BDTRInitStruct);
	TIM_BDTRConfig(TIM8, &TIM_BDTRInitStruct);
	TIM_CCPreloadControl(TIM8, ENABLE);
	TIM_CtrlPWMOutputs(TIM8, ENABLE);

  TIM_Cmd(TIM8, ENABLE);
}

//Set RGB LED Color
void SetLEDColor(int r, int g, int b)
{
	RGBLEDInit();
	TIM_SetCompare1(TIM8, r); //red
	TIM_SetCompare2(TIM8, b); //green
	TIM_SetCompare3(TIM8, g); //blue
}
