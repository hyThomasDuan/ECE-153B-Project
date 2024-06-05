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
	// [TODO]
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	//Set USART2 source as system clock 
	RCC->CCIPR |= RCC_CCIPR_USART1SEL_0;
	RCC->CCIPR &= ~RCC_CCIPR_USART1SEL_1;
	
}

void UART2_Init(void) {
	// [TODO]
	
	//Enable USART2 Clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	//Set USART2 source as system clock 
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_0;
	RCC->CCIPR &= ~RCC_CCIPR_USART2SEL_1;
	
}

void UART1_GPIO_Init(void) {
	// [TODO]
		// Enable GPIO Clocks
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

		// GPIO Mode
		GPIOB->MODER &= ~GPIO_MODER_MODE6_0;
		GPIOB->MODER |= GPIO_MODER_MODE6_1;
	
		GPIOB->MODER &= ~GPIO_MODER_MODE7_0;
		GPIOB->MODER |= GPIO_MODER_MODE7_1;
	
		//Set I/O output speed value as very high speed
		GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED6;
		GPIOB->OTYPER &= ~GPIO_OTYPER_OT6;
	
		GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED7;
		GPIOB->OTYPER &= ~GPIO_OTYPER_OT7;
	
		// GPIO Push-Pull: Pull-up
		GPIOB->PUPDR |= GPIO_PUPDR_PUPDR6_0;
		GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR6_1;
		
		GPIOB->PUPDR |= GPIO_PUPDR_PUPDR7_0;
		GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR7_1;
	
		// Alternative Function
		//GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL2;
		GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_0;
		GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_1;
		GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_2;
		GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL6_3;
		
		//GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL3;
		GPIOB->AFR[0] |= GPIO_AFRL_AFSEL7_0;
		GPIOB->AFR[0] |= GPIO_AFRL_AFSEL7_1;
		GPIOB->AFR[0] |= GPIO_AFRL_AFSEL7_2;
		GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL7_3;

}

void UART2_GPIO_Init(void) {
	// [TODO]
		// Enable GPIO Clocks
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

		// GPIO Mode
		GPIOA->MODER &= ~GPIO_MODER_MODE2_0;
		GPIOA->MODER |= GPIO_MODER_MODE2_1;
	
		GPIOA->MODER &= ~GPIO_MODER_MODE3_0;
		GPIOA->MODER |= GPIO_MODER_MODE3_1;
	
		//Set I/O output speed value as very high speed
		GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED2;
		GPIOA->OTYPER &= ~GPIO_OTYPER_OT2;
	
		GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED3;
		GPIOA->OTYPER &= ~GPIO_OTYPER_OT3;
	
		// GPIO Push-Pull: Pull-up
		GPIOA->PUPDR |= GPIO_PUPDR_PUPDR2_0;
		GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR2_1;
		
		GPIOA->PUPDR |= GPIO_PUPDR_PUPDR3_0;
		GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR3_1;
	
		// Alternative Function
		//GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL2;
		GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_0;
		GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_1;
		GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_2;
		GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL2_3;
		
		//GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL3;
		GPIOA->AFR[0] |= GPIO_AFRL_AFSEL3_0;
		GPIOA->AFR[0] |= GPIO_AFRL_AFSEL3_1;
		GPIOA->AFR[0] |= GPIO_AFRL_AFSEL3_2;
		GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL3_3;
}

void USART_Init(USART_TypeDef* USARTx) {
	// [TODO]
	
	//Disable USARTx 
	USARTx -> CR1 &= ~USART_CR1_UE;
	
	//Set word length to 8 bits
	USARTx -> CR1 &= ~USART_CR1_M;
	
	//Set  over-sampling to 16
	USARTx -> CR1 &= ~USART_CR1_OVER8;
	
	//Set stop bits 1
	USARTx -> CR2 &= ~USART_CR2_STOP_0;
	USARTx -> CR2 &= ~USART_CR2_STOP_1;
	
	//Set Baud rate
	USARTx -> BRR = 8333UL;
	
	//Enable transmit and receive
	USARTx -> CR1 |= USART_CR1_TE;
	USARTx -> CR1 |= USART_CR1_RE; 
 	
	//Enable USARTx
	USARTx -> CR1 |= USART_CR1_UE;
	
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
