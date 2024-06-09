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

uint8_t volatile Data_Receive; // for I2C

static char buffer[IO_SIZE];

void UART_onInput(char* inputs, uint32_t size) {
	//do write motor commands
	//sprintf(buffer, inputs);
	char data1 = inputs[0];
	if (data1 == '1' || data1 == 'r')
		{
			setDire(1);
			sprintf(buffer, "Clockwise\n");
			
		}
		else if (data1 == '2' || data1 == 'l')
		{
			setDire(2);
			sprintf(buffer, "Counter-clockwise\n");
		}
		else
		{
			setDire(3);
		  sprintf(buffer, "Stopping\n");
		}
		UART_print(buffer);
}

int main(void) {
	// Switch System Clock = 80 MHz

	System_Clock_Init();
	SysTick_Init();

	LED_Init();
	Motor_Init();
	/*
	UART1_Init();
	UART1_GPIO_Init();
	
	DMA_Init_UARTx(DMA1_Channel4, USART1);
	Init_DMA1_Channel4_IRQn();
  Init_USART1_IRQn();
	setSRC_DMA_Channel(DMA1_Channel4);
	*/
	
	
	UART2_Init();
	UART2_GPIO_Init();
	
	DMA_Init_UARTx(DMA1_Channel7, USART2);
	Init_DMA1_Channel7_IRQn();
  Init_USART2_IRQn();
	setSRC_DMA_Channel(DMA1_Channel7);
	
	
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
	UART_print(buffer);
	
	
/*	
	while(1)
	{
		
//		sprintf(buffer, "Program Starts.\r\n");
//	  UART_print(buffer);

		
		LED_Toggle();
		delay(1000);
	}
 */
 
 	I2C_GPIO_Init();
	I2C_Initialization();
 
 /* I2C TEST
	uint8_t SecondaryAddress;
//	uint8_t Data_Receive;
	uint8_t Data_Send;
	uint8_t Data_St[1];
	uint8_t Data_Rc[1];
	while(1) {	
		// Determine Secondary Address
		//
		// Note the "<< 1" must be present because bit 0 is treated as a don't care in 7-bit addressing mode
		SecondaryAddress = 0b1001000 << 1; // STUB - Fill in correct address 
		
		// [TODO] - Get Temperature
		// 
		// First, send a command to the sensor for reading the temperature
		// Next, get the measurement
		
		Data_St[0] = 0x01;
		Data_St[0] = 0x00;
		
		Data_Send = 0x00;
		
		
		I2C_SendData(I2C1, SecondaryAddress, Data_St, 2UL);

		I2C_ReceiveData(I2C1, SecondaryAddress, &Data_Receive, 1UL);
		
		// [TODO] - Print Temperature to Termite
		
		// Some delay
		for(int i = 0; i < 50000; ++i); 
		
		sprintf(buffer, "%d C\n", Data_Receive);
		UART_print(buffer);
	}
*/
	
	SPI1_GPIO_Init();
	SPI1_Init();
	initAcc();
	
	double x,y,z;

/* SPI Test
	while(1) {
		readValues(&x, &y, &z);
		sprintf(buffer,"Acceleration: %.2f, %.2f, %.2f\r\n", x*(4E-3), y*(4E-3), z*(4E-3));
		delay(1000);
		UART_print(buffer);
	}
*/
}


