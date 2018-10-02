#ifndef UTILITIES_H_
#define UTILITIES_H_

#include "stm32f30x.h"
#include "StringClass.h"

extern volatile uint32_t TimeDelayMS;
extern volatile uint32_t TimeDelayUS;
extern volatile bool StartDelay;

bool HasFlag(uint32_t mask, uint32_t flag);
void Delay(uint32_t nTime);
void DelayMicro(uint32_t nTime);
void LEDInit();
void SetLEDState(bool ON);
void LEDToggle();
bool GetLEDState();
bool IsStrEqual(char* str1, const char* str2);
bool IsStrEqual(const char* str1, const char* str2);
uint32_t GetClockFreq();
void RGBLEDInit();
void SetLEDColor(int r, int g, int b);

#define LED_Red 100, 0, 0
#define LED_Green 0, 100, 0
#define LED_Blue 0, 0, 100
#define LED_Off 0, 0, 0
#define LED_White 100, 100, 100
#define LED_Yellow 100, 35, 0
#define LED_Orange 100, 10, 0
#define LED_Magenta 100, 0, 100
#define LED_Cyan 0, 100, 100

#endif