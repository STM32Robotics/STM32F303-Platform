#include "RS232.h"

volatile void (*RS232Invoker)(char*, int) = 0;

void RS232Init()
{
	//Literally change the pins/uart number and it'll work
	//USART1 PA2 and PA3 do not work, they are not physically connected on the Nucleo

	USART_InitTypeDef usartStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef gpioStruct;
	gpioStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5; //PA4/PA5
	gpioStruct.GPIO_Speed = GPIO_Speed_50MHz;
	gpioStruct.GPIO_Mode = GPIO_Mode_AF;
	gpioStruct.GPIO_OType = GPIO_OType_PP;
	gpioStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &gpioStruct);

	//Important to define the alternate function
	//Must use pin source not actual pin value
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_7);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_7);
	
	//Enable UART clock attached to pins
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	usartStruct.USART_BaudRate = 115200;
	usartStruct.USART_WordLength = USART_WordLength_8b;  
	usartStruct.USART_StopBits = USART_StopBits_1;   
	usartStruct.USART_Parity = USART_Parity_No ;
	usartStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usartStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1, &usartStruct); //Initialize it
	USART_Cmd(USART1, ENABLE); //Enable the UART
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //Enable receieve interrupting
	
	//Enable the actual interrupt
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
}

void RS232Send(char ch)
{
	while(!USART_GetFlagStatus(USART1, USART_FLAG_TXE)); //Check to see if we can send
	USART_SendData(USART1, ch);
}

unsigned int RSDelayTimer = 5;
void RS232SendString(const char* str)
{
	Delay(RSDelayTimer); //Mandatory if RS232SendString is called in succession otherwise one character gets skipped
	for(int i = 0; i < strlen(str); i++)
	{
		RS232Send(str[i]);
		Delay(RSDelayTimer);
	}
	RS232Send('\r');
}

void RS232SendString(TString str)
{
	Delay(RSDelayTimer); //Mandatory if RS232SendString is called in succession otherwise one character gets skipped
	for(int i = 0; i < str.GetLength(); i++)
	{
		RS232Send(str[i]);
		Delay(RSDelayTimer);
	}
	RS232Send('\r');
}

void RS232SendString(char* str)
{
	Delay(RSDelayTimer); //Mandatory if RS232SendString is called in succession otherwise one character gets skipped
	for(int i = 0; i < strlen(str); i++)
	{
		RS232Send(str[i]);
		Delay(RSDelayTimer);
	}
	RS232Send('\r');
}

TString DataBuffer;
TString LastData;
char data;
extern "C"
{
	void USART1_IRQHandler()
	{
		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //Check if we can read
		{
			data = (char)USART_ReceiveData(USART1);
			if(data == '\r') //End of string
			{
				if(DataBuffer.GetLength() == 0) //Scrap empty buffer and don't bother
				{
					DataBuffer.Clear();
					return;
				}
				if(DataBuffer == LastData)
				{
					DataBuffer.Clear();
					return;
				}
				LastData = DataBuffer;
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