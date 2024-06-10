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

uint8_t volatile user_priority = 0;

uint8_t volatile message_pending = 0;

uint8_t volatile door_status = 0; //0 for closed, 1 for open

void UART_onInput(char* inputs, uint32_t size) {
	//do write motor commands
	//sprintf(buffer, inputs);
	char data1 = inputs[0];
	if (data1 == '1' || data1 == 'r')
		{
			setDire(1);
			sprintf(buffer, "Opening\n");
			UART_print(buffer);
			sprintf(buffer, " ");
			//while accel not in position
			//setDire(3);
		}
		else if (data1 == '2' || data1 == 'l')
		{
			setDire(2);
			sprintf(buffer, "Closing\n");
			UART_print(buffer);
			sprintf(buffer, " ");
			//while accel not in position
			//setDire(3);
		}
		else
		{
			setDire(3);
		  sprintf(buffer, "STOP\n");
			UART_print(buffer);
			sprintf(buffer, " ");
		}
		UART_print(buffer);
}

int main(void) {
	// Switch System Clock = 80 MHz

	System_Clock_Init();
	SysTick_Init();

	LED_Init();
	Motor_Init();
	
	//Put initial motor door reset here...

	I2C_GPIO_Init();
	I2C_Initialization();

  SPI1_GPIO_Init();
	SPI1_Init();
	initAcc();
	
	uint8_t SecondaryAddress;
	uint8_t Data_Send;
	uint8_t Data_St[1];
	uint8_t Data_Rc[1];
	
	double x,y,z;
	
	readValues(&x, &y, &z);

	if(z*(4E-3) > 0)
		setDire(1);

	while(z*(4E-3) > 0)
		readValues(&x, &y, &z);
	
	setDire(3);
	
	// After initial setup, enable USART
	
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
	
	//START I2C SETUP
	// Determine Secondary Address (I2C)
	// Note the "<< 1" must be present because bit 0 is treated as a don't care in 7-bit addressing mode
	SecondaryAddress = 0b1001000 << 1; // STUB - Fill in correct address 

	// Set command for reading the temperature (I2C)
		
	Data_St[0] = 0x01;
	Data_St[0] = 0x00;
		
	Data_Send = 0x00;
	
	//END I2C Setup
	
	sprintf(buffer, "Program Starts.\r\n");
	UART_print(buffer);
	
	while(1)
	{
		
			if ((message_pending == 1) && (getDire() == 3) )
			{
				sprintf(buffer, "Door Opened\r\n");
				UART_print(buffer);
				message_pending = 0;
				
				if (user_priority == 1)
				{
					 doorLockout(1);
					 user_priority = 0;
				}
				
			}
		  if ((message_pending == 2) && (getDire() == 3) )
			{
				sprintf(buffer, "Door Closed\r\n");
				UART_print(buffer);
				message_pending = 0;
				
				if (user_priority == 1)
				{
					 doorLockout(1);
					 user_priority = 0;
				}
			}
			
			
		
		// START I2C
		
		I2C_SendData(I2C1, SecondaryAddress, Data_St, 2UL);
		I2C_ReceiveData(I2C1, SecondaryAddress, &Data_Receive, 1UL);
		
		// Print Temperature to Termite
		
		// Some delay
		//for(int i = 0; i < 50000; ++i); 
		
		if(doorLockout(0) && (Data_Receive > 23) && (door_status == 0) && (user_priority == 0))
		{
		  sprintf(buffer, "%d C\n", Data_Receive);
		  UART_print(buffer);
			message_pending = 1;
			setDire(2);
		}
		
		if(doorLockout(0) && (Data_Receive < 19) && (door_status == 1) && (user_priority == 0))
		{
			message_pending = 2;
			setDire(1);
		}
		
		// END I2C
		
		// START SPI
		readValues(&x, &y, &z);
		sprintf(buffer,"Acceleration: %.2f, %.2f, %.2f\r\n", x*(4E-3), y*(4E-3), z*(4E-3));
		delay(1000);
		UART_print(buffer);
		// END SPI
		
		LED_Toggle();
		delay(1000);
	}
 
}


