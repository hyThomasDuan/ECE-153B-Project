/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Project
 */

#include "stm32l476xx.h"
#include "motor.h"

static const uint32_t MASK = 0;//TODO
static const uint32_t HalfStep[8] = {
		GPIO_ODR_OD5 | GPIO_ODR_OD9, // A, B'
		GPIO_ODR_OD5, // A
		GPIO_ODR_OD5 | GPIO_ODR_OD8, // A, B
		GPIO_ODR_OD8, // B
		GPIO_ODR_OD6 | GPIO_ODR_OD8, // A', B
		GPIO_ODR_OD6, // A';
		GPIO_ODR_OD6 | GPIO_ODR_OD9, // A', B'
		GPIO_ODR_OD9}; // B'

static volatile int8_t dire = 3;
static volatile uint8_t step = 0;

void Motor_Init(void) {	
		// Enable HSI
    ///RCC->CR |= ((uint32_t)RCC_CR_HSION);
    //while ( (RCC->CR & (uint32_t) RCC_CR_HSIRDY) == 0 );

    // Select HSI as system clock source
    //RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
		//RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSI;
    //while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) == 0 );

    // Enable GPIO Clock
    // [TODO]
				RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

    // Initialize Motor
    // [TODO]
				GPIOC->MODER |= GPIO_MODER_MODE5_0;
				GPIOC->MODER &= ~GPIO_MODER_MODE5_1;
				GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED5;
				GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED5;
				GPIOC->OTYPER &= ~GPIO_OTYPER_OT5;
				GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR5_0;
				GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR5_1;

				GPIOC->MODER |= GPIO_MODER_MODE6_0;
				GPIOC->MODER &= ~GPIO_MODER_MODE6_1;
				GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED6;
				GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED6;
				GPIOC->OTYPER &= ~GPIO_OTYPER_OT6;
				GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR6_0;
				GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR6_1;
				
				GPIOC->MODER |= GPIO_MODER_MODE8_0;
				GPIOC->MODER &= ~GPIO_MODER_MODE8_1;
				GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED8;
				GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED8;
				GPIOC->OTYPER &= ~GPIO_OTYPER_OT8;
				GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR8_0;
				GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR8_1;
				
				GPIOC->MODER |= GPIO_MODER_MODE9_0;
				GPIOC->MODER &= ~GPIO_MODER_MODE9_1;
				GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED9;
				GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED9;
				GPIOC->OTYPER &= ~GPIO_OTYPER_OT9;
				GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR9_0;
				GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR9_1;
				
    // Initialize User Button
				GPIOC->MODER &= ~GPIO_MODER_MODE13_0;
				GPIOC->MODER &= ~GPIO_MODER_MODE13_1;
				GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR13_0;
				GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR13_1;
				
}

void rotate(void) {
	if (dire == 1){ //clockwise
		if(step < 7){
				step++;
		}else{
				step = 0;
		}
	}else{ //counter rotate
			if(step > 0){ 
					step--;
			}else{
					step = 7;
			}
		}
	GPIOC->ODR = HalfStep[step]; // rotate 1 step
}

void setDire(int8_t direction) {
	 dire = direction; // 1=clock, 2=counter, 3=stop
}

int8_t getDire(void) {
	 return dire;
}
	



	


