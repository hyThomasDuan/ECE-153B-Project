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

uint8_t volatile t_hPoll = 0;
uint8_t volatile t_cPoll = 0;

int8_t volatile t_previous = 0;
int8_t volatile t_changePoll = 0;

void UART_onInput(char* inputs, uint32_t size) {
	//do write motor commands
	//sprintf(buffer, inputs);
	char data1 = inputs[0];
	if (data1 == '1' && (door_status == 0||door_status == 3||door_status == 4))
		{
			door_status = 4;
			setDire(1);
			sprintf(buffer, "Opening\r\n");
			UART_print(buffer);
			//sprintf(buffer, " ");
			message_pending = 1;
			user_priority = 1;
			t_hPoll = 0;
			t_cPoll = 0;		
		}
		else if (data1 == '2' && (door_status == 1||door_status == 3||door_status == 4))
		{
			door_status = 4;
			setDire(2);
			sprintf(buffer, "Closing\r\n");
			UART_print(buffer);
			sprintf(buffer, " ");
			//while accel not in position
			message_pending = 2;
			user_priority = 1;
			t_hPoll = 0;
			t_cPoll = 0;		
		}
		else if (data1 == '3')
		{
			door_status = 3;
			setDire(3);
			message_pending = 3;
		  sprintf(buffer, "STOP\r\n");
			UART_print(buffer);
			user_priority = 0;
			doorLockout(1);
			//sprintf(buffer, " ");
			//message_pending = 3;
			
		}else{
			//door_status = 3;
			//setDire(3);
			if (data1 == '1'){
					sprintf(buffer, "Door is already opened.\r\n");
					UART_print(buffer);
			}else if (data1 == '2'){
					sprintf(buffer, "Door is already closed.\r\n");
					UART_print(buffer);
			}else{
					sprintf(buffer, "Invalid Input\r\n");
					UART_print(buffer);
			}
			//sprintf(buffer, " ");
		}
		//UART_print(buffer);
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
	//setDire(3);
	if(z*(4E-3) > 0.50)
		setDire(2); //close if not when start
	


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
			if ((message_pending == 1) && (getDire() == 3) && (door_status != 3) )
			{
				sprintf(buffer, "Door Opened\r\n");
				UART_print(buffer);
				message_pending = 0;
				door_status = 1;
				
				if (user_priority == 1)
				{
					 doorLockout(1);
					 user_priority = 0;
				}
				
			}
		  else if ((message_pending == 2) && (getDire() == 3) && (door_status != 3) )
			{
				sprintf(buffer, "Door Closed\r\n");
				UART_print(buffer);
				message_pending = 0;
				door_status = 0;
				
				if (user_priority == 1)
				{
					 doorLockout(1);
					 user_priority = 0;
				}
			}
		  else if ((message_pending == 3) && (getDire() == 3) && (door_status == 3) )
			{
				sprintf(buffer, "Door Stopped\r\n");
				UART_print(buffer);
				message_pending = 0;
				door_status = 3;
				user_priority = 0;
			}else{}
			
			//debug print
			//sprintf(buffer, "c:%d h:%d user:%d lock:%d door:%d dire:%d msg:%d\n", t_cPoll, t_hPoll, user_priority, doorLockout(0), door_status, getDire(), message_pending);
		  //UART_print(buffer);
			
		
		// START I2C
		
		I2C_SendData(I2C1, SecondaryAddress, Data_St, 2UL);
		I2C_ReceiveData(I2C1, SecondaryAddress, &Data_Receive, 1UL);
		
		// Print Temperature to Termite
		//sprintf(buffer, "%d C\n", (int8_t)Data_Receive);
		//UART_print(buffer);
		
		// Some delay
		//for(int i = 0; i < 50000; ++i); 
			
		// Temperature Polling
		if ((Data_Receive > 24) && (Data_Receive < 128)){
			t_hPoll++;
			t_cPoll = 0;	
		}else if((Data_Receive < 10) || (Data_Receive > 128)){
			t_cPoll++;
			t_hPoll = 0;
		}else{
			t_hPoll = 0;
			t_cPoll = 0;		
		}
		
		//temperature change anti-spam
		t_changePoll++;
		if (t_changePoll >= 10){
				if (t_previous != (int8_t)Data_Receive){
						t_changePoll = 0;
						t_previous = (int8_t)Data_Receive;
						sprintf(buffer, "Temperature is changed to %d C.\n", (int8_t)Data_Receive);
						UART_print(buffer);
				}
				else{
						t_changePoll = 0;
				}
		}

			
		if(doorLockout(0) && (t_hPoll >= 30) && (door_status == 0||door_status == 3) && (user_priority == 0))
		{
		  sprintf(buffer, "Temperature: %d C. Too hot. Door opening.\n", (int8_t)Data_Receive);
		  UART_print(buffer);
			
			message_pending = 1;
			t_hPoll = 0;
			t_cPoll = 0;
			setDire(1);
			door_status = 4;
		}
		
		if(doorLockout(0) && (t_cPoll >= 30) && (door_status == 1||door_status == 3) && (user_priority == 0))
		{ // ~200 means underflow of the data, door_status should ==1
			sprintf(buffer, "Temperature: %d C. Too cold. Door closing.\n", (int8_t)Data_Receive);
		  UART_print(buffer);
			message_pending = 2;
			t_cPoll = 0;
			t_hPoll = 0;
			setDire(2);
			door_status = 4;

		}
		
		// END I2C
		
		// START SPI
		//readValues(&x, &y, &z);
		//sprintf(buffer,"Acceleration: %.2f, %.2f, %.2f, Temperature: %d C\r\n", x*(4E-3), y*(4E-3), z*(4E-3), (int8_t)Data_Receive);
		// delay(1000);
		//UART_print(buffer);
		// END SPI
		
		LED_Toggle();
		delay(100);
	}
 
}


