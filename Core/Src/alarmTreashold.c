#include "alarmTreashold.h"

volatile uint8_t checkBuzzer = 0;
volatile uint8_t checkPeriod = 0;

void alarm(uint8_t trashold)
{

	if(!checkBuzzer)
		{
			HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_1);
			TIM1 -> ARR = 612-1;
			checkBuzzer = 1;
			checkPeriod++;
		}
		else
		{
			HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_1);
			if(checkPeriod == 0 && trashold == 1)
			{
				checkBuzzer = 0;
				return;
			}
			else if(checkPeriod > 0 && checkPeriod <= 1 && trashold == 2)
			{
				checkBuzzer = 0;
				return;
			}
			else
			{
				TIM1 -> ARR = 10000-1;
				checkPeriod = 0;
				checkBuzzer = 0;
			}
		}

}
