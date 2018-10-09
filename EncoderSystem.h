#ifndef ENCODER_H_
#define ENCODER_H_

#include "stm32f30x.h"

void EncoderInit();
uint32_t GetWaveSpeed();
uint32_t GetSpeed();
void Timer3Handler();

#endif