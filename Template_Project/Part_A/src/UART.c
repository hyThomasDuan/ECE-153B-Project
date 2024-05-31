#include "UART.h"

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

uint8_t USART_Read (USART_TypeDef * USARTx) {
	// SR_RXNE (Read data register not empty) bit is set by hardware
	while (!(USARTx->ISR & USART_ISR_RXNE));  // Wait until RXNE (RX not empty) bit is set
	// USART resets the RXNE flag automatically after reading DR
	return ((uint8_t)(USARTx->RDR & 0xFF));
	// Reading USART_DR automatically clears the RXNE flag 
}

void USART_Write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes) {
	int i;
	// TXE is cleared by a write to the USART_DR register.
	// TXE is set by hardware when the content of the TDR 
	// register has been transferred into the shift register.
	for (i = 0; i < nBytes; i++) {
		while (!(USARTx->ISR & USART_ISR_TXE));   	// wait until TXE (TX empty) bit is set
		// Writing USART_DR automatically clears the TXE flag 	
		USARTx->TDR = buffer[i] & 0xFF;
		USART_Delay(40000);
	}
	while (!(USARTx->ISR & USART_ISR_TC));   		  // wait until TC bit is set
	USARTx->ISR &= ~USART_ISR_TC;
}   

void USART_Delay(uint32_t us) {
	uint32_t time = 100*us/7;    
	while(--time);   
}