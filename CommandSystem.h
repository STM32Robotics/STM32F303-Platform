#ifndef COMMAND_SYS_H
#define COMMAND_SYS_H

#include "stm32f30x.h"
#include "Utilities.h"
#include "RS232.h"
#include <string.h>

#define CommandSize 128
#define CommandSizeQueue 64

void InitCommandSystem();
void AddCommandHandler(void (*f)(TString&), const char* cmd);
unsigned int ExecuteAllQueueCommands();
void AddCommandToQueue(TString &data);

#endif