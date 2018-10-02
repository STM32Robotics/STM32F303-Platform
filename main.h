#ifndef _MAIN_H_
#define _MAIN_H_

#include "stm32f30x.h"
#include "SysTick.h"
#include "USARTSystem.h"
#include "StepperMotor.h"
#include "RS232.h"
#include "CommandSystem.h"
#include "TimerCallback.h"
#include "Utilities.h"

extern volatile void (*LoopFunc)();

#endif