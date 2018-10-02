#ifndef RS232_H_
#define RS232_H_

#include "stm32f30x_usart.h"
#include "Utilities.h"
#include "CommandSystem.h"

void RS232Init();
void RS232Send(char ch);
void RS232SendString(const char*);
void RS232SendString(char*);
void RS232SendString(TString);
#endif