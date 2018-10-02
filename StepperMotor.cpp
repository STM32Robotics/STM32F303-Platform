#include "StepperMotor.h"

void StepperInit()
{
	GPIO_InitTypeDef GPIOStep;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	GPIOStep.GPIO_Mode = GPIO_Mode_OUT;
	GPIOStep.GPIO_OType = GPIO_OType_PP;
	GPIOStep.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIOStep.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOStep.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIOStep);
}