#include "CommandSystem.h"

void (*CommandTable[CommandSize])(TString args) = {0};
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

void AddCommandHandler(void (*f)(TString), const char* cmd)
{
	CommandTable[CurrentCommandCount] = f;
	CommandTableKeys[CurrentCommandCount] = cmd;
	CurrentCommandCount++;
}

void ExecuteAllQueueCommands()
{
	for(unsigned int i = 0; i < CommandSizeQueue; i++)
	{
		if(QueuedCommands[i].GetLength() != 0)
		{
			bool found = false;
			for(int j = 0; j < CommandSize; j++)
			{
				if(QueuedCommands[i].DoesWordEqualTo(1, CommandTableKeys[j]))
				{
					CommandTable[j](QueuedCommands[i]);
					QueuedCommands[i].Clear();
					found = true;
				}
			}
			if(!found)
			{
				TString str = "Command:Unknown (";
				str += QueuedCommands[i];
				str += ") ";
				
				QueuedCommands[i].Clear();
				RS232SendString(str);
			}
		}
	}
}

void AddCommandToQueue(TString data)
{
	for(unsigned int i = 0; i < CommandSizeQueue; i++)
	{
		if(QueuedCommands[i].GetLength() == 0)
		{
			QueuedCommands[i] = data;
			return;
		}
	}
}