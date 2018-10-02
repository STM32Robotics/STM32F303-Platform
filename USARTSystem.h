#ifndef _USARTSYSTEM_H_
#define _USARTSYSTEM_H_

#include "stm32f30x_usart.h"
#include "Utilities.h"
#include <stdlib.h>

//Prototypes
void USARTInit();
void USARTSend(char);
void SetUSARTInvoker(volatile void(*f)(const char*));
extern "C" { void USART2_IRQHandler(); }
void BufferInit();
extern volatile bool IsUSARTReady;

#endif