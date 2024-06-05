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

char volatile data1;

int main(void) {
	char ch;
	// Switch System Clock = 80 MHz
	System_Clock_Init(); 
	Motor_Init();
	SysTick_Init();
	UART2_GPIO_Init();
	UART2_Init();
	USART_Init(USART2);//TODO
	setDire(3);
	printf("Program Starts.\r\n");
	while(1) {

		scanf("%s",&data1);
//	for(int i = 0; i<100; i++);
//	printf("%c\n",data1);
		
		if (data1 == '1' || data1 == 'r')
		{
			setDire(1);
			printf("Clockwise\n");
			
		}
		else if (data1 == '2' || data1 == 'l')
		{
			setDire(2);
			printf("Counterclockwise\n");
		}
		else
		{
			setDire(3);
			printf("Stopping\n");
		}
	}
}


