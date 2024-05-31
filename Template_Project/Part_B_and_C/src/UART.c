/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Project
 */


#include "UART.h"
#include "DMA.h"

static volatile DMA_Channel_TypeDef * tx;
static volatile char inputs[IO_SIZE];
static volatile uint8_t data_t_0[IO_SIZE];
static volatile uint8_t data_t_1[IO_SIZE];
static volatile uint8_t input_size = 0;
static volatile uint8_t pending_size = 0;
static volatile uint8_t * active = data_t_0;
static volatile uint8_t * pending = data_t_1;

#define SEL_0 1
#define BUF_0_EMPTY 2
#define BUF_1_EMPTY 4
#define BUF_0_PENDING 8
#define BUF_1_PENDING 16

void transfer_data(char ch);
void on_complete_transfer(void);

void UART1_Init(void) {
	//TODO
}

void UART2_Init(void) {
	//TODO
}

void UART1_GPIO_Init(void) {
	//TODO
}

void UART2_GPIO_Init(void) {
	//TODO
}

void USART_Init(USART_TypeDef * USARTx) {
	//TODO
}

/**
 * This function accepts a string that should be sent through UART
*/
void UART_print(char* data) {
	//TODO

	//Transfer char array to buffer
	//Check DMA status. If DMA is ready, send data
	//If DMA is not ready, put the data aside
}

/**
 * This function should be invoked when a character is accepted through UART
*/
void transfer_data(char ch) {
	//TODO
	// Append character to input buffer.
	// If the character is end-of-line, invoke UART_onInput
}

/**
 * This function should be invoked when DMA transaction is completed
*/
void on_complete_transfer(void) {
	//TODO
	// If there are pending data to send, switch active and pending buffer, and send data
}

void USART1_IRQHandler(void){
	//TODO
	// When receive a character, invoke transfer_data
	// When complete sending data, invoke on_complete_transfer
}

void USART2_IRQHandler(void){
	//TODO
	// When receive a character, invoke transfer_data
	// When complete sending data, invoke on_complete_transfer
}
