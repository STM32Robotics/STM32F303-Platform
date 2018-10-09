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

// Macro to grab only the lower byte of an argument
#define LOW( value )    ((value) & 0xFFFF)
// Macro to set specified bits
#define SET_BITS( port, bits )            ((port) |= (bits))
// Macro to clear specified bits
#define CLR_BITS( port, bits )            ((port) &= LOW(~(bits)))
//Macro to flip specified bits
#define FLIP_BITS( port, bits )           ((port) ^= (bits))
// Macro to force the bits identified by mask to take on the values of the matching bits in value
#define FORCE_BITS( port, mask, value)    (port) = ((port) & LOW(~(mask)) | ((value) & (mask)))
#define FORCE_WORD( port, mask, value)    (port) = ((port) & ((~(mask)) & 0xFFFF) | ((value) & (mask)))

#endif