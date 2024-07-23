#ifndef __ALARM_TRSHLD__
#define __ALARM_TRSHLD__

#define firstTrashold  1
#define secondTrashold 2

#include "stm32f1xx.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

void alarm(uint8_t trashold);

#endif
