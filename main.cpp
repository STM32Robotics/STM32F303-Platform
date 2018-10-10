#include "main.h"
#include "DCMotorSystem.h"

//https://www.keil.com/update/sw/mdk/5.26

void SetLEDColorEx(TString arg)
{
	if(arg == "green")
	{
		SetLEDColor(LED_Green);
		RS232SendString("Command:Successful (led green)");
	}
	else if(arg == "red")
	{
		SetLEDColor(LED_Red);
		RS232SendString("Command:Successful (led red)");
	}
	else if(arg == "blue")
	{
		SetLEDColor(LED_Blue);
		RS232SendString("Command:Successful (led blue)");
	}
	else if(arg == "yellow")
	{
		SetLEDColor(LED_Yellow);
		RS232SendString("Command:Successful (led yellow)");
	}
	else if(arg == "white")
	{
		SetLEDColor(LED_White);
		RS232SendString("Command:Successful (led white)");
	}
	else if(arg == "magenta")
	{
		SetLEDColor(LED_Magenta);
		RS232SendString("Command:Successful (led magenta)");
	}
	else if(arg == "cyan")
	{
		SetLEDColor(LED_Cyan);
		RS232SendString("Command:Successful (led cyan)");
	}
	else if(arg == "off")
	{
		SetLEDColor(LED_Off);
		RS232SendString("Command:Successful (led off)");
	}
}

void LEDCallback(TString msg)
{
	bool FoundCommand = false;
	if(msg.DoesWordEqualTo(2, "green"))
	{
		FoundCommand = true;
		SetLEDColorEx("green");
	}
	else if(msg.DoesWordEqualTo(2, "blue"))
	{
		FoundCommand = true;
		SetLEDColorEx("blue");
	}
	else if(msg.DoesWordEqualTo(2, "red"))
	{
		FoundCommand = true;
		SetLEDColorEx("red");
	}
	else if(msg.DoesWordEqualTo(2, "off"))
	{
		FoundCommand = true;
		SetLEDColorEx("off");
	}
	else if(msg.DoesWordEqualTo(2, "yellow"))
	{
		FoundCommand = true;
		SetLEDColorEx("yellow");
	}
	else if(msg.DoesWordEqualTo(2, "white"))
	{
		FoundCommand = true;
		SetLEDColorEx("white");
	}
	else if(msg.DoesWordEqualTo(2, "magenta"))
	{
		FoundCommand = true;
		SetLEDColorEx("magenta");
	}
	else if(msg.DoesWordEqualTo(2, "cyan"))
	{
		FoundCommand = true;
		SetLEDColorEx("cyan");
	}
	else
	{
		TString str = "Command:Unsuccessful (";
		str += msg;
		str += ")";
		RS232SendString(str);
		return;
	}
}

volatile bool DoTask = false;

int step = 1;

void Timer2CH1Callback()
{
	LEDToggle();
}

void ResetCommand(TString args)
{
	SetLEDColorEx("red");
	RS232SendString("Resetting...");
	//Clean up
	Delay(2500);
	NVIC_SystemReset();
}

void PingPongCommand(TString args)
{
	RS232SendString("Pong");
}

void ReportOnline(TString args)
{
	RS232SendString("Is Online");
}

void LCDCommand(TString args)
{
	if(args.DoesWordEqualTo(2, "clear"))
	{
		LCDClearLine(First);
	}
	else
	{
		LCDClearLine(First);
		args.RemoveFirstWords(1);
		LCDSendString(args);
	}
}

extern "C"
{
	void HardFault_Handler()
	{
		RS232SendString("HardFault:Handler");
	}
	
	void MemManage_Handler()
	{
		RS232SendString("MemManage:Handler");
	}
	
	void BusFault_Handler()
	{
		RS232SendString("BusFault:Handler");
	}
	
	void UsageFault_Handler()
	{
		RS232SendString("UsageFault:Handler");
	}
	
	void SVC_Handler()
	{
		RS232SendString("SVC:Handler");
	}
	
	void DebugMon_Handler()
	{
		RS232SendString("DebugMon:Handler");
	}
	
	void PendSV_Handler()
	{
		RS232SendString("PendSV:Handler");
	}
}

int SpeedM1 = 0;
int SpeedM2 = 0;
motor_dir Dir1 = FORWARD;
motor_dir Dir2 = FORWARD;

void MotorCommand(TString args)
{
	SpeedM1 = args.GetIntFromWord(2);
	SpeedM2 = args.GetIntFromWord(3);

	if(SpeedM1 < 0)
	{
		Dir1 = BACKWARD;
		SpeedM1 = -SpeedM1;
	}
	else
	{
		Dir1 = FORWARD;
	}
	
	if(SpeedM2 < 0)
	{
		Dir2 = BACKWARD;
		SpeedM2 = -SpeedM2;
	}
	else
	{
		Dir2 = FORWARD;
	}
}

//96 bits
//Unique ID found at 0x1FFFF7AC
//Address offset 0x00 32 bits -> BCD format
//Address offset 0x04 32 bits -> 32-39 -> Wafer Number -> 40-63 -> Lot number
//Address offset 0x08 32 bits -> 64-95 -> Lot number

#define STM32_UUID ((uint32_t*)0x1FFFF7AC)
int main()
{
	SystemInit();
	SetLEDColor(LED_White);
	SysTickInit();
	DCMotorInit();
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
	LCDInit();
	Delay(100);
	SetLEDColor(LED_Yellow);
	Delay(100);
	RS232SendString("Boot:Successful");
	SetLEDColorEx("off");
	LEDToggle();
	unsigned int i = 0;
	
	TString arg = "UID: ";
	uint32_t idPart1 = STM32_UUID[0];
	uint32_t idPart2 = STM32_UUID[1];
	uint32_t idPart3 = STM32_UUID[2];
	arg += idPart1;
	arg += idPart2;
	arg += idPart3;
	RS232SendString(arg);
	//Delay(5000);
	
	AddCommandHandler(&LEDCallback, "led");                                                
	AddCommandHandler(&ResetCommand, "reset");
	AddCommandHandler(&PingPongCommand, "ping");
	AddCommandHandler(&ReportOnline, "online");
	AddCommandHandler(&LCDCommand, "lcd");
	AddCommandHandler(&MotorCommand, "motor");
	
	AddTimerCallback("TIM8CH1", &Timer2CH1Callback);
	LCDSendString("Booted");
	
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
		DCMotor(M_ONE, SpeedM1, Dir1);
		DCMotor(M_TWO, SpeedM2, Dir2);
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