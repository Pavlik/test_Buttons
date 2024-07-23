#include "debounce.h"

volatile bool buttonState = 0;
extern volatile bool buttonPressed;


void getDataFromButton()
{
	buttonState = !HAL_GPIO_ReadPin(Button1_GPIO_Port, Button1_Pin);
	if(buttonState && !buttonPressed) //Если кнопка нажата и флаг нажатия не опущен считать кнопку нажатой, но не обрабатывать
	{
		//HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		buttonPressed = 1;
		EXTI->FTSR &= ~EXTI_RTSR_TR1;  // Отключаем прерывание по нисходящему фронту
		EXTI->RTSR |= EXTI_FTSR_TR1;   // Включаем прерывание по восходящему фронту
		TIM3 -> ARR = 20000;		   // Подобрать значение чтобы была 2 секунда
	}
	else if(!buttonState && !buttonPressed) //Если кнопка не нажата и флаг нажатия не опущен
	{
		HAL_TIM_Base_Stop_IT(&htim3);
	}
	else if(buttonState && buttonPressed) //Если кнопка нажата и флаг нажатия опущен считать кнопку удержанной
	{
		handleLongPress();
	}
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);  	// очищаем бит EXTI_PR
	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);	// очищаем бит NVIC_ICPRx
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);     // включаем внешнее прерывание
}

void handleShortPress()
{
	//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0);
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

	buttonPressed = 0;
	EXTI->FTSR |= EXTI_RTSR_TR1;  // Включаем прерывание по нисходящему фронту
	EXTI->RTSR &= ~EXTI_FTSR_TR1;
	TIM3 -> ARR = 1000;
	TIM3 -> CNT = 0;

	HAL_TIM_Base_Stop_IT(&htim3);
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);  		// очищаем бит EXTI_PR
	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);	// очищаем бит NVIC_ICPRx
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);     // включаем внешнее прерывание
}

void handleLongPress()
{
	//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

	buttonPressed = 0;
	EXTI->FTSR |= EXTI_RTSR_TR1;  // Включаем прерывание по нисходящему фронту
	EXTI->RTSR &= ~EXTI_FTSR_TR1;
	TIM3 -> ARR = 1000;

	HAL_TIM_Base_Stop_IT(&htim3);
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);  		// очищаем бит EXTI_PR
	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);	// очищаем бит NVIC_ICPRx
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);     // включаем внешнее прерывание
}




