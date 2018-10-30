#ifndef ENCODER_H_
#define ENCODER_H_

#include "stm32f30x.h"

#define TIM3_CNT_CLOCK 1000000
#define PRESCALE  (uint32_t)((SystemCoreClock / TIM3_CNT_CLOCK) - 1)
#define PERIOD 0x0000FFFF
#define VANE_NUM 27.0
#define GEAR_RATIO 22.5
#define WHEEL_ROTATION 16.65
#define NUM_US_IN_SEC 1000000.0

void EncoderInit();
uint32_t GetWaveSpeed();
uint32_t GetSpeed();
void Timer3Handler();

#endif