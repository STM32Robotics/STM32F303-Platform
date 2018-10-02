#include "RS232.h"

volatile void (*RS232Invoker)(char*, int) = 0;

void RS232Init()
{
	//Literally change the pins/uart number and it'll work
	//USART1 PA2 and PA3 do not work, they are not physically connected

	USART_InitTypeDef usartStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef gpioStruct;
	gpioStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	gpioStruct.GPIO_Speed = GPIO_Speed_50MHz;
	gpioStruct.GPIO_Mode = GPIO_Mode_AF;
	gpioStruct.GPIO_OType = GPIO_OType_PP;
	gpioStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &gpioStruct);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_7);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_7);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	usartStruct.USART_BaudRate = 115200;   
	usartStruct.USART_WordLength = USART_WordLength_8b;  
	usartStruct.USART_StopBits = USART_StopBits_1;   
	usartStruct.USART_Parity = USART_Parity_No ;
	usartStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usartStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1, &usartStruct);
	USART_Cmd(USART1, ENABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
}

void RS232Send(char ch)
{
	while(!USART_GetFlagStatus(USART2, USART_FLAG_TXE));
	USART_SendData(USART1, ch);
}

void RS232SendString(const char* str)
{
	for(int i = 0; i < strlen(str); i++)
	{
		RS232Send(str[i]);
		Delay(1);
	}
	RS232Send('\r');
}

void RS232SendString(TString str)
{
	for(int i = 0; i < str.GetLength(); i++)
	{
		RS232Send(str[i]);
		Delay(1);
	}
	RS232Send('\r');
}

void RS232SendString(char* str)
{
	for(int i = 0; i < strlen(str); i++)
	{
		RS232Send(str[i]);
		Delay(1);
	}
	RS232Send('\r');
}

TString DataBuffer;

extern "C"
{
	void USART1_IRQHandler()
	{
		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
		{
			char data = (char)USART_ReceiveData(USART1);
			if(data == '\r')
			{
				AddCommandToQueue(DataBuffer);
				DataBuffer.Clear();
			}
			else
			{
				DataBuffer += data;
			}
		}
	}
}