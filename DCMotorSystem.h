#ifndef DCMOTORSYSTEM_H_
#define DCMOTORSYSTEM_H_

#include "stm32f30x.h"

void InitPWMMotors();
int PWMInitDC(int pwm_freq);
void vMotorControlSetPwmValues(int32_t pwmA, int32_t pwmB);

#endif