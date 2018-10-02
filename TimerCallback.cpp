#include "TimerCallback.h"

void (*TimerTableFuncs[TimerSize])() = {0};
TString TimerTableKeys[TimerSize];
unsigned int TimerSizeCount = 0;

void InitTimerCallback()
{
	for(unsigned int i = 0; i < TimerSize; i++)
	{
		TimerTableFuncs[i] = 0;
		TimerTableKeys[i].Clear();
	}
}

//Timer = "TIM1CH2"
void AddTimerCallback(const char* timer, void(*func)())
{
	TimerTableFuncs[TimerSizeCount] = func;
	TimerTableKeys[TimerSizeCount] = timer;
	TimerSizeCount++;
}

void ExecuteTimerCallback(const char* timer)
{
	for(unsigned int i = 0; i < TimerSize; i++)
	{
		if(TimerTableKeys[i] == timer)
		{
			TimerTableFuncs[i]();
		}
	}
}

extern "C"
{
	void TIM1_CC_IRQHandler()
	{
		if(TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
		{
			TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
			ExecuteTimerCallback("TIM1CH1");
		}
		else if(TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET)
		{
			TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);
			ExecuteTimerCallback("TIM1CH2");
		}
		else if(TIM_GetITStatus(TIM1, TIM_IT_CC3) != RESET)
		{
			TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);
			ExecuteTimerCallback("TIM1CH3");
		}
		else if(TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET)
		{
			TIM_ClearITPendingBit(TIM1, TIM_IT_CC4);
			ExecuteTimerCallback("TIM1CH4");
		}
	}
	
	void TIM2_IRQHandler()
	{
		if(TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
		{
			TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
			ExecuteTimerCallback("TIM2CH1");
		}
		else if(TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)
		{
			TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
			ExecuteTimerCallback("TIM2CH2");
		}
		else if(TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET)
		{
			TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
			ExecuteTimerCallback("TIM2CH3");
		}
		else if(TIM_GetITStatus(TIM2, TIM_IT_CC4) != RESET)
		{
			TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
			ExecuteTimerCallback("TIM2CH4");
		}
	}

	void TIM3_IRQHandler()
	{
		if(TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
			ExecuteTimerCallback("TIM3CH1");
		}
		else if(TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
			ExecuteTimerCallback("TIM3CH2");
		}
		else if(TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
			ExecuteTimerCallback("TIM3CH3");
		}
		else if(TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
			ExecuteTimerCallback("TIM3CH4");
		}
	}

	void TIM4_IRQHandler()
	{
		if(TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
		{
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
			ExecuteTimerCallback("TIM4CH1");
		}
		else if(TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)
		{
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
			ExecuteTimerCallback("TIM4CH2");
		}
		else if(TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET)
		{
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
			ExecuteTimerCallback("TIM4CH3");
		}
		else if(TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET)
		{
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
			ExecuteTimerCallback("TIM4CH4");
		}
	}

	void TIM7_IRQHandler()
	{
		if(TIM_GetITStatus(TIM7, TIM_IT_CC1) != RESET)
		{
			TIM_ClearITPendingBit(TIM7, TIM_IT_CC1);
			ExecuteTimerCallback("TIM7CH1");
		}
		else if(TIM_GetITStatus(TIM7, TIM_IT_CC2) != RESET)
		{
			TIM_ClearITPendingBit(TIM7, TIM_IT_CC2);
			ExecuteTimerCallback("TIM7CH2");
		}
		else if(TIM_GetITStatus(TIM7, TIM_IT_CC3) != RESET)
		{
			TIM_ClearITPendingBit(TIM7, TIM_IT_CC3);
			ExecuteTimerCallback("TIM7CH3");
		}
		else if(TIM_GetITStatus(TIM7, TIM_IT_CC4) != RESET)
		{
			TIM_ClearITPendingBit(TIM7, TIM_IT_CC4);
			ExecuteTimerCallback("TIM7CH4");
		}
	}
	
	void TIM8_CC_IRQHandler()
	{
		if(TIM_GetITStatus(TIM8, TIM_IT_CC1) != RESET)
		{
			TIM_ClearITPendingBit(TIM8, TIM_IT_CC1);
			ExecuteTimerCallback("TIM8CH1");
		}
		else if(TIM_GetITStatus(TIM8, TIM_IT_CC2) != RESET)
		{
			TIM_ClearITPendingBit(TIM8, TIM_IT_CC2);
			ExecuteTimerCallback("TIM8CH2");
		}
		else if(TIM_GetITStatus(TIM8, TIM_IT_CC3) != RESET)
		{
			TIM_ClearITPendingBit(TIM8, TIM_IT_CC3);
			ExecuteTimerCallback("TIM8CH3");
		}
		else if(TIM_GetITStatus(TIM8, TIM_IT_CC4) != RESET)
		{
			TIM_ClearITPendingBit(TIM8, TIM_IT_CC4);
			ExecuteTimerCallback("TIM8CH4");
		}
	}
	
	void TIM20_CC_IRQHandler()
	{
		if(TIM_GetITStatus(TIM20, TIM_IT_CC1) != RESET)
		{
			TIM_ClearITPendingBit(TIM20, TIM_IT_CC1);
			ExecuteTimerCallback("TIM20CH1");
		}
		else if(TIM_GetITStatus(TIM20, TIM_IT_CC2) != RESET)
		{
			TIM_ClearITPendingBit(TIM20, TIM_IT_CC2);
			ExecuteTimerCallback("TIM20CH2");
		}
		else if(TIM_GetITStatus(TIM20, TIM_IT_CC3) != RESET)
		{
			TIM_ClearITPendingBit(TIM20, TIM_IT_CC3);
			ExecuteTimerCallback("TIM20CH3");
		}
		else if(TIM_GetITStatus(TIM20, TIM_IT_CC4) != RESET)
		{
			TIM_ClearITPendingBit(TIM20, TIM_IT_CC4);
			ExecuteTimerCallback("TIM20CH4");
		}
	}
};