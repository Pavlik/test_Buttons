#include "debounce.h"

volatile bool buttonState = false;

//struct buttons button1 = {Button1_Pin, Button1_GPIO_Port, false, EXTI_FTSR_TR1, EXTI_RTSR_TR1};


void getDataFromButton(struct buttons * button)
{
	buttonState = !HAL_GPIO_ReadPin(button->port, button->pin);
	if(buttonState && !(button->buttonPressed)) //Если кнопка нажата и флаг нажатия не опущен считать кнопку нажатой, но не обрабатывать
	{
		button->buttonPressed = true;
		EXTI->FTSR &= ~(button->fallingBit);  // Отключаем прерывание по нисходящему фронту
		EXTI->RTSR |= button->risingBit;   // Включаем прерывание по восходящему фронту
		TIM3 -> ARR = 20000;		   // Подобрать значение чтобы была 2 секунда
	}
	else if(!buttonState && !(button->buttonPressed)) //Если кнопка не нажата и флаг нажатия не опущен
	{
		HAL_TIM_Base_Stop_IT(&htim3);
	}
	else if(buttonState && (button->buttonPressed)) //Если кнопка нажата и флаг нажатия опущен считать кнопку удержанной
	{
		handleLongPress(&button);
	}
	__HAL_GPIO_EXTI_CLEAR_IT(button->pin);  	// очищаем бит EXTI_PR
	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);	// очищаем бит NVIC_ICPRx
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);     // включаем внешнее прерывание
}

void handleShortPress(struct buttons ** button)
{

	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

	(*button)->buttonPressed = false;
	EXTI->FTSR |= (*button)->fallingBit;  // Включаем прерывание по нисходящему фронту
	EXTI->RTSR &= ~((*button)->risingBit);
	TIM3 -> ARR = 1000;
	TIM3 -> CNT = 0;

	HAL_TIM_Base_Stop_IT(&htim3);
	__HAL_GPIO_EXTI_CLEAR_IT((*button)->pin);  		// очищаем бит EXTI_PR
	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);	// очищаем бит NVIC_ICPRx
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);     // включаем внешнее прерывание
}

void handleLongPress(struct buttons ** button)
{
	//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

	(*button)->buttonPressed = false;
	EXTI->FTSR |= (*button)->fallingBit;  // Включаем прерывание по нисходящему фронту
	EXTI->RTSR &= ~(*button)->risingBit;
	TIM3 -> ARR = 1000;

	HAL_TIM_Base_Stop_IT(&htim3);
	__HAL_GPIO_EXTI_CLEAR_IT((*button)->pin);  		// очищаем бит EXTI_PR
	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);	// очищаем бит NVIC_ICPRx
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);     // включаем внешнее прерывание
}

void callBackButton(struct buttons * button)
{
	if(!(button->buttonPressed))
	{
		HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
		HAL_TIM_Base_Start_IT(&htim3);
	}
	else if(button->buttonPressed)
	{
		HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
		handleShortPress(&button);
	}
	else return;
}



