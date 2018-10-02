#include "main.h"

//https://www.keil.com/update/sw/mdk/5.26

void LEDCallback(TString msg)
{
	bool FoundCommand = false;
	if(msg.DoesWordEqualTo(2, "green"))
	{
		FoundCommand = true;
		SetLEDColor(LED_Green);
	}
	else if(msg.DoesWordEqualTo(2, "blue"))
	{
		FoundCommand = true;
		SetLEDColor(LED_Blue);
	}
	else if(msg.DoesWordEqualTo(2, "red"))
	{
		FoundCommand = true;
		SetLEDColor(LED_Red);
	}
	else if(msg.DoesWordEqualTo(2, "off"))
	{
		FoundCommand = true;
		SetLEDColor(LED_Off);
	}
	else if(msg.DoesWordEqualTo(2, "yellow"))
	{
		FoundCommand = true;
		SetLEDColor(LED_Yellow);
	}
	else if(msg.DoesWordEqualTo(2, "white"))
	{
		FoundCommand = true;
		SetLEDColor(LED_White);
	}
	else if(msg.DoesWordEqualTo(2, "magenta"))
	{
		FoundCommand = true;
		SetLEDColor(LED_Magenta);
	}
	else if(msg.DoesWordEqualTo(2, "cyan"))
	{
		FoundCommand = true;
		SetLEDColor(LED_Cyan);
	}
	else
	{
		TString str = "Command:Unsuccessful (";
		str += msg;
		str += ")";
		RS232SendString(str);
		return;
	}
	
	TString str = "Command:Successful (";
	str += msg;
	str += ")";
	RS232SendString(str);
}

volatile bool DoTask = false;

int step = 1;

void Timer2CH1Callback()
{
	LEDToggle();
}

void ResetCommand(TString args)
{
	RS232SendString("Resetting...");
	SetLEDColor(LED_Red);
	//Clean up
	Delay(2500);
	NVIC_SystemReset();
}

void PingPongCommand(TString args)
{
	RS232SendString("Pong");
}

int main()
{
	SystemInit();
	SetLEDColor(LED_White);
	SysTickInit();
	SetLEDColor(LED_Red);
	Delay(100);
	InitCommandSystem();
	SetLEDColor(LED_Green);
	Delay(100);
	InitTimerCallback();
	SetLEDColor(LED_Blue);
	Delay(100);
	RS232Init();
	SetLEDColor(LED_Magenta);
	Delay(100);
	RS232SendString("Boot:Successful");
	SetLEDColor(LED_Off);
	LEDToggle();
	unsigned int i = 0;
	
	                                                                              
	AddCommandHandler(&ResetCommand, "reset");
	AddCommandHandler(&PingPongCommand, "ping");
	
	
	AddTimerCallback("TIM8CH1", &Timer2CH1Callback);
	
	while(true)
	{
		Delay(1);
		ExecuteAllQueueCommands();
		i++;
		if(i >= 50)
		{
			LEDToggle();
			i = 0;
		}
		/*if(step == 1)
			GPIOC->ODR = GPIO_Pin_6;
		else if(step == 2)
			GPIOC->ODR = (GPIO_Pin_6 | GPIO_Pin_7);
		else if(step == 3)
			GPIOC->ODR = (GPIO_Pin_7);
		else if(step == 4)
			GPIOC->ODR = (GPIO_Pin_7 | GPIO_Pin_8);
		else if(step == 5)
			GPIOC->ODR = (GPIO_Pin_8);
		else if(step == 6)
			GPIOC->ODR = (GPIO_Pin_8 | GPIO_Pin_9);
		else if(step == 7)
			GPIOC->ODR = (GPIO_Pin_9);
		else if(step == 8)
			GPIOC->ODR = (GPIO_Pin_9 | GPIO_Pin_6);
		
		step++;
		if(step > 8)
			step = 1;*/
	}
	return 0;
}