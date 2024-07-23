#ifndef __DEBOUNCE__
#define __DEBOUNCE__

#include "stm32f1xx.h"
#include "stdbool.h"
#include "main.h"

extern TIM_HandleTypeDef htim3;

void getDataFromButton();
void handleShortPress();
void handleLongPress();

#endif
