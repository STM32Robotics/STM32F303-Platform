#include "TimerCallback.h"
#include "ServoSystem.h"

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
		
		if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
		{
			TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
			ExecuteTimerCallback("TIM1");
		}
	}
	
	#define __HAL_TIM_GET_FLAG(__HANDLE__, __FLAG__)       (((__HANDLE__)->SR &(__FLAG__)) == (__FLAG__))
	#define __HAL_TIM_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__) ((((__HANDLE__)->DIER & (__INTERRUPT__)) == (__INTERRUPT__)) ? SET : RESET)
	#define __HAL_TIM_CLEAR_FLAG(__HANDLE__, __FLAG__)       ((__HANDLE__)->SR = ~(__FLAG__))
	void TIM2_IRQHandler()
	{
		if(__HAL_TIM_GET_FLAG(TIM2, TIM_FLAG_CC4) != RESET)
		{
			if(__HAL_TIM_GET_IT_SOURCE(TIM2, TIM_IT_CC4) != RESET)
			{
				__HAL_TIM_CLEAR_FLAG(TIM2, TIM_FLAG_CC4);
				
				if((TIM2->CCMR2 & TIM_CCMR2_CC4S) != 0x00U)
				{
					
				}
				else
				{
					//SetLEDColor(LED_Blue);
					Timer2PWM();
					//ExecuteTimerCallback("TIM2CH4");
				}
			}
		}
		
		if(__HAL_TIM_GET_FLAG(TIM2, TIM_SR_UIF) != RESET)
		{
			if(__HAL_TIM_GET_IT_SOURCE(TIM2, TIM_DIER_UIE) !=RESET)
			{ 
				__HAL_TIM_CLEAR_FLAG(TIM2, TIM_SR_UIF);
			}
		}
		
		if(__HAL_TIM_GET_FLAG(TIM2, TIM_SR_BIF) != RESET)
		{
			if(__HAL_TIM_GET_IT_SOURCE(TIM2, TIM_DIER_BIE) !=RESET)
			{ 
				__HAL_TIM_CLEAR_FLAG(TIM2, TIM_SR_BIF);
			}
		}
		
		return;
		//SetLEDColor(LED_Magenta);
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
			if((TIM2->CCMR2 & TIM_CCMR2_CC4S) != 0x00U)
			{
				
			}
			else
			{
				//SetLEDColor(LED_Blue);
				ExecuteTimerCallback("TIM2CH4");
			}
		}
		if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
		{
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
			ExecuteTimerCallback("TIM2");
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
		ExecuteTimerCallback("TIM3");
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
		
		if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) 
		{
			TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
			ExecuteTimerCallback("TIM4");
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
		ExecuteTimerCallback("TIM7");
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
		ExecuteTimerCallback("TIM8");
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
		ExecuteTimerCallback("TIM20");
	}
};