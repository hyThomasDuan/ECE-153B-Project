/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Project
 */

#include "stm32l476xx.h"
#include "SysClock.h"
#include "SysTimer.h"
#include "UART.h"
#include "motor.h"
#include <stdio.h>

int main(void) {
	char ch;
	// Switch System Clock = 80 MHz
	System_Clock_Init(); 
	Motor_Init();
	SysTick_Init();
	UART2_GPIO_Init();
	UART2_Init();
	USART_Init(USART2);//TODO
	
	printf("Program Starts.\r\n");
	while(1) {
		//TODO
	}
}


