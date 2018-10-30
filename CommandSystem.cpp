#include "CommandSystem.h"

void (*CommandTable[CommandSize])(TString& args) = {0};
TString CommandTableKeys[CommandSize];
TString QueuedCommands[CommandSizeQueue];
unsigned int CurrentCommandCount = 0;

void InitCommandSystem()
{
	for(int i = 0; i < CommandSize; i++)
	{
		CommandTable[i] = 0;
		CommandTableKeys[i].Clear();
	}
}

void AddCommandHandler(void (*f)(TString&), const char* cmd)
{
	CommandTable[CurrentCommandCount] = f;
	CommandTableKeys[CurrentCommandCount] = cmd;
	CurrentCommandCount++;
}

TString ErrBuffStr;
unsigned int ExecuteAllQueueCommands()
{
	unsigned int amount = 0;
	for(unsigned int i = 0; i < CommandSizeQueue; i++)
	{
		if(QueuedCommands[i].GetLength() != 0)
		{
			bool found = false;
			for(int j = 0; j < CurrentCommandCount; j++)
			{
				if(QueuedCommands[i].DoesWordEqualTo(1, CommandTableKeys[j]))
				{
					CommandTable[j](QueuedCommands[i]);
					amount++;
					QueuedCommands[i].Clear();
					found = true;
				}
			}
			if(!found)
			{
				ErrBuffStr = "Command:Unknown (";
				ErrBuffStr += QueuedCommands[i];
				ErrBuffStr += ") ";
				QueuedCommands[i].Clear();
				RS232SendString(ErrBuffStr);
			}
		}
	}
	return amount;
}

void AddCommandToQueue(TString &data)
{
	int trackedI = -1;
	for(unsigned int i = 0; i < CommandSizeQueue; i++)
	{
		if(QueuedCommands[i] == data)
		{
			return;
		}
		else if(QueuedCommands[i].GetLength() == 0 && trackedI == -1)
		{
			trackedI = i;
		}
	}
	if(trackedI == -1)
		return;
	if(QueuedCommands[trackedI].GetLength() == 0)
	{
		QueuedCommands[trackedI] = data;
		return;
	}
}