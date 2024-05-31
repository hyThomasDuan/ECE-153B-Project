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
#include "LED.h"
#include "DMA.h"
#include "UART.h"
#include "motor.h"
#include "SPI.h"
#include "I2C.h"
#include "accelerometer.h"
#include <stdio.h>

static char buffer[IO_SIZE];

void UART_onInput(char* inputs, uint32_t size) {
	//TODO
}

int main(void) {
	// Switch System Clock = 80 MHz
	System_Clock_Init(); 
	Motor_Init();
	SysTick_Init();
	UART2_Init();
	LED_Init();	
	SPI1_GPIO_Init();
	SPI1_Init();
	initAcc();
	I2C_GPIO_Init();
	I2C_Initialization();
	
	sprintf(buffer, "Program Starts.\r\n");
	UART_print(buffer);
	while(1) {
		//TODO
		LED_Toggle();
		delay(1000);
	}
}


