#include "stm32f30x.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_rcc.h"
#include "StringClass.h"
#include "Utilities.h"
#include <stdarg.h>

typedef enum
{
	CMD = 0,
	DATA
} LCD_SEND_TYPE;

typedef enum e_lcdline
{
	First,
	Second,
	Third,
	Fourth
} LCDLINE;

#define LCD_RW_MASK GPIO_Pin_15
#define LCD_RS_MASK GPIO_Pin_13
#define LCD_E_MASK GPIO_Pin_12

#define LCD_BUSY_FLAG GPIO_Pin_7
#define LCD_BUS_MASK GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7
#define LCD_PORT GPIOB

#define LCD_CLEAR_MASK 0x01
#define LCD_HOME_MASK 0x02
#define LCD_NEW_LINE 0xC0
#define LCD_CMD_FUNCTION 0x20
#define LCD_FUNCTION_4BIT 0x00
#define LCD_FUNCTION_2LINES 0x08
#define LCD_FUNCTION_5X8FONT 0x00
#define LCD_DISPLAY 0x08
#define LCD_DISPLAY_ON 0x04
#define LCD_DISPLAY_OFF 0x00
#define LCD_NOCURSOR 0x00
#define LCD_NOBLINK 0x00
#define LCD_ENTRY 0x04
#define LCD_INC 0x02
#define LCD_MOVE_CURSOR 0x00
#define LCD_CGADDR 0x40
#define LCD_RTN_HOME 0x02

#define LOW( value )    ((value) & 0xFFFF)
#define FORCE_BITS( port, mask, value)    (port) = ((port) & LOW(~(mask)) | ((value) & (mask)))

#define TIM1_CNT_CLOCK_LCD 3200000
#define PRESCALE_LCD (uint32_t)((SystemCoreClock / TIM1_CNT_CLOCK_LCD) - 1)
#define PERIOD_LCD 99
#define MAX_LCD_BUFSIZ 40U

void LCDInit();
void LCDSendChar(uint16_t letter);
void LCDSendString (const char *str);
void LCDSendString(TString &str);
void LCDClear(void);
void LCDSetPos (LCDLINE lineSel, unsigned char offset);
void LCDClearLine(LCDLINE lineSel);
void LCDPrintf(char *fmt, ... );