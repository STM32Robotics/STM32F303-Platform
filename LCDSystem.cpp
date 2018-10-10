#include "LCDSystem.h"

static GPIO_InitTypeDef pb_InitStruct;
static GPIO_InitTypeDef pa_InitStruct;
static TIM_TimeBaseInitTypeDef tim1_initStruct;
static TIM_OCInitTypeDef t1oc_config;
static TIM_BDTRInitTypeDef bdtr_initStruct;

static void LCD_Send_HNibble(uint16_t inData)
{
    volatile uint16_t tmp = GPIOB->ODR;
    
    GPIO_WriteBit(LCD_PORT, LCD_E_MASK, Bit_SET);
    FORCE_BITS(GPIOB->ODR, LCD_BUS_MASK, inData);
    GPIO_WriteBit(LCD_PORT, LCD_E_MASK, Bit_RESET);
}

static void LCD_Send_LNibble(uint16_t inData)
{
    volatile uint16_t tmp = GPIOB->ODR;

    GPIO_WriteBit(LCD_PORT, LCD_E_MASK, Bit_SET);
    FORCE_BITS(GPIOB->ODR, LCD_BUS_MASK, (inData << 4));
    GPIO_WriteBit(LCD_PORT, LCD_E_MASK, Bit_RESET);
}

static void init_lcd_busy_flag (void)
{
    pb_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    pb_InitStruct.GPIO_Pin = LCD_BUSY_FLAG;
    pb_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    pb_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
    
    GPIO_Init(GPIOB, &pb_InitStruct);    
}

static void restore_lcd_data (void)
{
    pb_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    pb_InitStruct.GPIO_Pin = LCD_BUSY_FLAG;
    pb_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    pb_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
    
    GPIO_Init(GPIOB, &pb_InitStruct);   
}
static void readBusyFlag(void) 
{
   uint16_t checkBusy;
   init_lcd_busy_flag();

   GPIO_WriteBit(LCD_PORT, LCD_E_MASK, Bit_RESET);
   GPIO_WriteBit(LCD_PORT,LCD_RS_MASK, Bit_RESET);  
   GPIO_WriteBit(LCD_PORT,LCD_RW_MASK, Bit_SET);
   
   do 
   {
    GPIO_WriteBit(LCD_PORT, LCD_E_MASK, Bit_SET);  
    checkBusy = GPIO_ReadInputData(GPIOB);
    GPIO_WriteBit(LCD_PORT, LCD_E_MASK, Bit_RESET);
    
    GPIO_WriteBit(LCD_PORT, LCD_E_MASK, Bit_SET);  
    GPIO_ReadInputData(GPIOB);
    GPIO_WriteBit(LCD_PORT, LCD_E_MASK, Bit_RESET);
    
   }while(checkBusy & LCD_BUSY_FLAG);
   restore_lcd_data();   
}

static void LCD_Send(uint16_t val, LCD_SEND_TYPE type)
{ 
    GPIO_WriteBit(LCD_PORT, LCD_E_MASK, Bit_RESET);

    if (type == CMD)
        GPIO_WriteBit(LCD_PORT,LCD_RS_MASK, Bit_RESET);
    else
        GPIO_WriteBit(LCD_PORT,LCD_RS_MASK, Bit_SET);
    
    GPIO_WriteBit(LCD_PORT,LCD_RW_MASK, Bit_RESET);
    LCD_Send_HNibble(val);
    LCD_Send_LNibble(val);
    readBusyFlag();
}

void LCDInit()
{
    /* Initialize PWM signal for controlling LCD Backlight */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    pa_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    pa_InitStruct.GPIO_Pin = GPIO_Pin_10;
    pa_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    pa_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
    pa_InitStruct.GPIO_OType = GPIO_OType_PP;   
 
    GPIO_Init(GPIOA, &pa_InitStruct);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_6);
   
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_TIM1, ENABLE);
    
    tim1_initStruct.TIM_Prescaler = PRESCALE;
    tim1_initStruct.TIM_Period = PERIOD;
    tim1_initStruct.TIM_ClockDivision = 0;
    tim1_initStruct.TIM_RepetitionCounter = 0;
    tim1_initStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &tim1_initStruct);
    
    /* OC Generic */
    t1oc_config.TIM_OCMode = TIM_OCMode_PWM1;
    t1oc_config.TIM_OCPolarity = TIM_OCPolarity_High;
    
    /* PWM SPECIFIC */
    //t1oc_config.TIM_OCNPolarity = TIM_OCNPolarity_High;
    //t1oc_config.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    t1oc_config.TIM_OCIdleState = TIM_OCIdleState_Reset;
    t1oc_config.TIM_Pulse = (uint32_t)(PERIOD /2);
    
    TIM_OC3Init(TIM1, &t1oc_config);

    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM1, ENABLE);
    
    /*Advance TIM SPECIFICS */
    TIM_BDTRStructInit(&bdtr_initStruct);
    TIM_BDTRConfig(TIM1, &bdtr_initStruct);
    TIM_CCPreloadControl(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    TIM_Cmd(TIM1, ENABLE);
    
    /* Initialize LCD GPIO*/
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    
    pb_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    pb_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 |  GPIO_Pin_7 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
    pb_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    pb_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
    
    GPIO_Init(GPIOB, &pb_InitStruct);
    
	LCD_Send_HNibble(0x30);
    Delay(5);

	LCD_Send_HNibble(0x30);
    Delay(5);

	LCD_Send_HNibble(0x30);
	Delay(5);;

	LCD_Send_HNibble(0x20);
    Delay(5);

    
    LCD_Send(LCD_CMD_FUNCTION|LCD_FUNCTION_4BIT|LCD_FUNCTION_2LINES|LCD_FUNCTION_5X8FONT, CMD);
    LCD_Send(LCD_DISPLAY|LCD_DISPLAY_OFF, CMD);
    LCD_Send(LCD_CLEAR_MASK, CMD);
    LCD_Send(LCD_ENTRY|LCD_MOVE_CURSOR|LCD_INC, CMD);
    LCD_Send(LCD_DISPLAY|LCD_DISPLAY_ON|LCD_NOCURSOR|LCD_NOBLINK, CMD);
}

void LCDSendChar(uint8_t letter)
{
    LCD_Send((uint16_t)letter, DATA);
}

void LCDSendString (char *str)
{
   unsigned char i = 0;
   
   while(str[i] != '\0') {
       LCDSendChar(str[i]);
       i++;
   }
}

void LCDSendString (TString str)
{
	for(int i = 0; i < str.GetLength(); i++)
	{
		LCDSendChar(str[i]);
	}
}

void LCDSetPos(LCDLINE lineSel , unsigned char offset)
{
	if (lineSel == Second) {
		if (offset <= 19) {
			LCD_Send (0xC0 + offset, CMD);
		} else {
			LCD_Send(0xC0, CMD);
		}
	} else if (lineSel == Third) {
		if (offset <= 19) {
			LCD_Send(0x94 + offset, CMD);
		} else {
			LCD_Send(0x94, CMD);
		}
	} else if (lineSel == Fourth) {
		if (offset <= 19) {
			LCD_Send(0xD4 + offset, CMD);
		} else {
			LCD_Send(0xD4, CMD);
		}
	} else {
		if (offset == 0) {
			LCD_Send(0x80, CMD);
		} else {
			LCD_Send(0x80 + offset, CMD);
		}
	}
}

void LCDClearLine (LCDLINE lineSel)
{
  unsigned char i = 0;

	if (lineSel == First) {
		LCDSetPos (First, 0);

		while ( i <= 19) {
			LCDSendChar(' ');
			i = i + 1;
		}

		LCDSetPos (First, 0);

	} else if (lineSel == Second) {
		LCDSetPos (Second, 0);

		while ( i <= 19) {
			LCDSendChar(' ');
			i = i + 1;
		}

		LCDSetPos (Second, 0);

	} else if (lineSel == Third) {
		LCDSetPos (Third, 0);

		while ( i <= 19) {
			LCDSendChar(' ');
			i = i + 1;
		}

		LCDSetPos (Third, 0);

	} else if (lineSel == Fourth) {
  
		LCDSetPos (Fourth, 0);

		while ( i <= 19) {
			LCDSendChar(' ');
			i = i + 1;
		}

		LCDSetPos (Fourth, 0);

	} else {}
}

void LCD_Clear_Screen (void)
{
	LCD_Send(LCD_CLEAR_MASK, CMD);
}

void LCD_Printf(char *fmt, ... ) {
  va_list args;
   //
  // need to guarantee that buffer big enough for any string
  char buffer[MAX_LCD_BUFSIZ];    

  va_start( args, fmt );
  (void)vsprintf(buffer, fmt, args);
  va_end( args );
 
  LCDSendString(buffer);
}