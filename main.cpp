#include "main.h"
#include "DCMotorSystem.h"
#include "StepperMotor.h"
#include "ServoSystem.h"
#include <cmath>

//https://www.keil.com/update/sw/mdk/5.26

void SetLEDColorEx(TString &arg)
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

TString ledStr;
void SetLEDColorEx(const char* str)
{
	ledStr = str;
	SetLEDColorEx(ledStr);
}

void LEDCallback(TString &msg)
{
	bool FoundCommand = false;
	SetLEDColorEx(msg);
}

volatile bool DoTask = false;

int step = 1;

void Timer2CH1Callback()
{
	LEDToggle();
}

void ResetCommand(TString &args)
{
	SetLEDColorEx("red");
	LCDClearLine(First);
	LCDClearLine(Second);
	LCDSetPos(First, 0);
	LCDSendString("Resetting...\0");
	RS232SendString("Resetting...");
	//Clean up
	Delay(2500);
	NVIC_SystemReset();
}

void PingPongCommand(TString &args)
{
	RS232SendString("Pong");
}

bool LinkedConnection = false;
void ReportOnline(TString &args)
{
	LinkedConnection = true;
	RS232SendString("online");
}

void LCDCommand(TString &args)
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
		LCDClearLine(First);
		LCDSetPos(First, 0);
		LCDSendString("HardFault");
	}
	
	void MemManage_Handler()
	{
		LCDClearLine(First);
		LCDSetPos(First, 0);
		LCDSendString("HardF");
	}
	
	void BusFault_Handler()
	{
		LCDClearLine(First);
		LCDSetPos(First, 0);
		LCDSendString("BusF");
	}
	
	void UsageFault_Handler()
	{
		LCDClearLine(First);
		LCDSetPos(First, 0);
		LCDSendString("UsageF");
	}
	
	void SVC_Handler()
	{
		LCDClearLine(First);
		LCDSetPos(First, 0);
		LCDSendString("SVCF");
	}
	
	void DebugMon_Handler()
	{
		LCDClearLine(First);
		LCDSetPos(First, 0);
		LCDSendString("DebugMonF");
	}
	
	void PendSV_Handler()
	{
		LCDClearLine(First);
		LCDSetPos(First, 0);
		LCDSendString("PendSVF");
	}
}

int SpeedM1 = 0;
int SpeedM2 = 0;
int prevM1 = 0;
int prevM2 = 0;
motor_dir Dir1 = FORWARD;
motor_dir Dir2 = FORWARD;

void MotorCommand(TString &args)
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

int STemp = 0;
int StepSize = HALF_STEP;
int IntendedDirection = DIR_LEFT;
int ActualDirection = DIR_LEFT;
int ActualStep = 195;
int LastStep = 195;
void StepperCommand(TString &args)
{
	if(args.DoesWordEqualTo(2, "init"))
	{
		DCMotor(M_ONE, 0, Dir1);
		DCMotor(M_TWO, 0, Dir2);
		StepperPosInit();
		ActualStep = 195;
	}
	else
	{
		STemp = args.GetIntFromWord(2);
		if(STemp < 0)
		{
			IntendedDirection = DIR_LEFT;
		}
		else
		{
			IntendedDirection = DIR_RIGHT;
		}
		
		STemp *= (double)((double)(39.0)/(double)(20.0));
	}
}

TString uidarg;
void RequestUID()
{
	//96 bits
	//Unique ID found at 0x1FFFF7AC
	//Address offset 0x00 32 bits -> BCD format
	//Address offset 0x04 32 bits -> 32-39 -> Wafer Number -> 40-63 -> Lot number
	//Address offset 0x08 32 bits -> 64-95 -> Lot number
	
	uidarg = "UID: ";
	uidarg += STM32_UUID[0];;
	uidarg += STM32_UUID[1];;
	uidarg += STM32_UUID[2];;
	RS232SendString(uidarg);
}
void GetUIDCommand(TString &args)
{
	RequestUID();
}

void ServoCMD(TString &args)
{
	Servo(args.GetIntFromWord(2));
}

bool IsRemote = true;

void SwitchType(TString &args)
{
	if(args.DoesWordEqualTo(2, "remote"))
	{
		IsRemote = true;
	}
	else
	{
		IsRemote = false;
	}
}

void FirstConnect(TString &args)
{
	RequestUID();
}

void SetStoredColor(int &r, int &g, int &b, int ra, int ga, int ba)
{
	r = ra;
	g = ga;
	b = ba;
}

TString con;
TString stepper;
TString motorSpd;
TString dab;
TString dab2;

unsigned int i = 0;
unsigned int j = 0;
unsigned int r = 0;
unsigned int m = 0;
int storedR = 0;
int storedG = 0;
int storedB = 0;
bool blinkToggle = false;
unsigned int del = 0;
int diff = 0;
unsigned int ins = 0;

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
	
	RequestUID();
	
	AddCommandHandler(&LEDCallback, "led");                                                
	AddCommandHandler(&ResetCommand, "reset");
	AddCommandHandler(&PingPongCommand, "ping");
	AddCommandHandler(&ReportOnline, "online");
	AddCommandHandler(&LCDCommand, "lcd");
	AddCommandHandler(&MotorCommand, "motor");
	AddCommandHandler(&StepperCommand, "stepper");
	AddCommandHandler(&ServoCMD, "servo");
	AddCommandHandler(&GetUIDCommand, "getuid");
	AddCommandHandler(&SwitchType, "type");
	AddCommandHandler(&FirstConnect, "handshake");
	LCDSendString("Booting...");
	SetLEDColor(LED_Yellow);
	StepperInit();
	LCDClearLine(First);
	LCDSendString("Servo Init");
	ServoInit();
	Delay(1000);
	Servo(90);
	StepperPosInit();
	LCDClearLine(First);
	LCDSendString("Booted");
	Delay(1000);
	LCDSetPos(Second, 0);
	LCDSendString("Connecting...");
	Delay(500);
	while(true)
	{
		DelayMicro(250);
		del++;
		if(del == 4)
		{
			del = 0;
			i++;
			j++;
			r++;
			m++;
		}
		ins += ExecuteAllQueueCommands();
		if(ins > 99999)
			ins = 0;
		
		if(i >= 25)
		{
			LEDToggle();
			i = 0;
		}
		if(m > 250)
		{
			if(prevM1 != SpeedM1 || prevM2 != SpeedM2)
			{
				motorSpd.Clear();
				motorSpd += "msp ";
				motorSpd += SpeedM1;
				if(Dir1 == 0)
					motorSpd += " 0 ";
				else
					motorSpd += " 1 ";
				motorSpd += SpeedM2;
				if(Dir2 == 0)
					motorSpd += " 0 ";
				else
					motorSpd += " 1 ";
				RS232SendString(motorSpd);
			}
			prevM1 = SpeedM1;
			prevM2 = SpeedM2;
			m = 0;
		}
		if(j >= 500)
		{
			if(!LinkedConnection)
			{
				LCDClearLine(Second);
				con.Clear();
				con = "Disconnected";
				LCDSendString(con);
				SpeedM1 = 0;
				SpeedM2 = 0;
				SetStoredColor(storedR, storedG, storedB, LED_Red);
			}
			else
			{
				LCDClearLine(Second);
				con.Clear();
				con = "Connected";
				LCDSendString(con);
				SetStoredColor(storedR, storedG, storedB, LED_Green);
				//SetLEDColor(LED_Green);
			}
			
			con.Clear();
			con = "CMD:";
			con += ins;
			LCDSetPos(First, 0);
			LCDSendString(con);
			
			if(IsRemote)
			{
				con.Clear();
				con = "Remote";
				LCDSetPos(First, 16 - con.GetLength());
				LCDSendString(con);
			}
			else
			{
				con.Clear();
				con = " Local";
				LCDSetPos(First, 16 - con.GetLength());
				LCDSendString(con);
			}
			
			LinkedConnection = false;
			
			/*stepper.Clear();
			stepper += (uint32_t)TIM2->CCR4;
			LCDClearLine(First);
			LCDSetPos(First, 10);
			LCDSendString(stepper);
			stepper.Clear();
			stepper += STemp;
			LCDSetPos(Second, 10);
			LCDSendString(stepper);*/
			j = 0;
		}
		
		if(!IsRemote)
		{
			r = 0;
			SetLEDColor(storedR, storedG, storedB);
		}
		
		if(r > 250)
		{
			if(blinkToggle)
				SetLEDColor(LED_Off);
			else
				SetLEDColor(storedR, storedG, storedB);
			blinkToggle = !blinkToggle;
			r = 0;
		}
		
		DCMotor(M_ONE, SpeedM1, Dir1);
		DCMotor(M_TWO, SpeedM2, Dir2);
		//continue;
		if(STemp == 0)
		{
			if(GetStepAmount() > 385)
			{
				diff = abs(GetStepAmount() - 385);
				Stepper(DIR_LEFT, HALF_STEP, diff, true);
			}
			else if(GetStepAmount() < 385)
			{
				diff = abs(GetStepAmount() - 385);
				Stepper(DIR_RIGHT, HALF_STEP, diff, true);
			}
		}
		else
		{
			Stepper(IntendedDirection, HALF_STEP, STemp, true);
		}
	}
	return 0;
}