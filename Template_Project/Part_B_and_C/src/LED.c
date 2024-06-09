/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Project
 */

#include "LED.h"

void LED_Init(void) {
    // Enable GPIO Clock
    // [TODO]
				RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    // Initialize Green LED
    // [TODO]
				GPIOA->MODER |= GPIO_MODER_MODE5_0;
				GPIOA->MODER &= ~GPIO_MODER_MODE5_1;
				GPIOA->OTYPER &= ~GPIO_OTYPER_OT5;
				GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR5_0;
				GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR5_1;
	
				//GPIOA->ODR &= ~GPIO_ODR_OD5;

}

void LED_Off(void) {

		GPIOA->ODR &= ~GPIO_ODR_OD5;
}

void LED_On(void) {
		GPIOA->ODR |= GPIO_ODR_OD5;
}

void LED_Toggle(void) {
	GPIOA->ODR ^= GPIO_ODR_OD5;
}
