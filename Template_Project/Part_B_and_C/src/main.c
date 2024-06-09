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
	//do write motor commands
	sprintf(buffer, inputs);
	
}

int main(void) {
	// Switch System Clock = 80 MHz

	System_Clock_Init();
	SysTick_Init();

	LED_Init();
	
	
	UART1_Init();
	UART1_GPIO_Init();
	
	DMA_Init_UARTx(DMA1_Channel4, USART1);
	Init_DMA1_Channel4_IRQn();
  Init_USART1_IRQn();
	setSRC_DMA_Channel(DMA1_Channel4);
	
	
	/*
	UART2_Init();
	UART2_GPIO_Init();
	
	DMA_Init_UARTx(DMA1_Channel7, USART2);
	Init_DMA1_Channel7_IRQn();
  Init_USART2_IRQn();
	setSRC_DMA_Channel(DMA1_Channel7);
	*/
	
	/*
	uint32_t BuffAdd = (uint32_t)&buffer2; 
	uint32_t DestAdd = (uint32_t)&USART2->TDR;
	DMA1_Channel7->CMAR = BuffAdd; //store data source address
	DMA1_Channel7->CPAR = DestAdd; //store data destination address
	
	
	DMA1_Channel7->CNDTR = 26UL; //number of data transfer	

	DMA1_Channel7->CCR |= DMA_CCR_EN; //enable DMA

	//USART2->TDR = 'D';
	
	//DMA1_Channel6->CCR &= ~DMA_CCR_EN; //disable DMA

	*/
	sprintf(buffer, "Initial\n");
	
	while(1)
	{
		/*
		sprintf(buffer, "Program Starts.\r\n");
	  UART_print(buffer);
		sprintf(buffer, "FUN\n");
	  UART_print(buffer);
		sprintf(buffer, "TIMES\n");
	  UART_print(buffer);
		sprintf(buffer, "FOR\n");
	  UART_print(buffer);
		sprintf(buffer, "ALL\n");
	  UART_print(buffer);	
    */
		
		UART_print(buffer);
		
		LED_Toggle();
		delay(1000);
	}

/*	
	Motor_Init();

	UART2_Init();
	LED_Init();	
	SPI1_GPIO_Init();
	SPI1_Init();
	initAcc();
	I2C_GPIO_Init();
	I2C_Initialization();
*/
	
/*	
	sprintf(buffer, "Program Starts.\r\n");
	UART_print(buffer);
	while(1) {
		//TODO
		LED_Toggle();
		delay(1000);
	}
*/
}


