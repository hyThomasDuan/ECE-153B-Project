#include "SPI.h"
#include "SysTimer.h"

void SPI1_GPIO_Init(void) {
	// Enable the GPIO Clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	// Set PA4, PB3, PB4, and PB5 to Alternative Functions, and configure their AFR to SPI1
	// Set GPIO Pins to: Very High Output speed, Output Type Push-Pull, and No Pull-Up/Down
	
	//Set pin mode
		GPIOA->MODER &= ~GPIO_MODER_MODE4_0;
		GPIOA->MODER |= GPIO_MODER_MODE4_1;
	
		GPIOB->MODER &= ~GPIO_MODER_MODE3_0;
		GPIOB->MODER |= GPIO_MODER_MODE3_1;
	
		GPIOB->MODER &= ~GPIO_MODER_MODE4_0;
		GPIOB->MODER |= GPIO_MODER_MODE4_1;
	
		GPIOB->MODER &= ~GPIO_MODER_MODE5_0;
		GPIOB->MODER |= GPIO_MODER_MODE5_1;
	
		//Set I/O output speed value as very high speed
		GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED4;
		GPIOA->OTYPER &= ~GPIO_OTYPER_OT4;
	
		GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED3;
		GPIOB->OTYPER &= ~GPIO_OTYPER_OT3;
		
		GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED4;
		GPIOB->OTYPER &= ~GPIO_OTYPER_OT4;
		
		GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED5;
		GPIOB->OTYPER &= ~GPIO_OTYPER_OT5;
	
	//GPIO Push-Pull: NPU NPD
	
		GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR4_0;
		GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR4_1;
	
		GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR3_0;
		GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR3_1;
	
		GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR4_0;
		GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR4_1;
		
		GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR5_0;
		GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR5_1;
	
	//GPIO Alternate Function

		GPIOA->AFR[0] |= GPIO_AFRL_AFSEL4_0;
		GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL4_1;
		GPIOA->AFR[0] |= GPIO_AFRL_AFSEL4_2;
		GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL4_3;
	
		GPIOB->AFR[0] |= GPIO_AFRL_AFSEL3_0;
		GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL3_1;
		GPIOB->AFR[0] |= GPIO_AFRL_AFSEL3_2;
		GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL3_3;
	
		GPIOB->AFR[0] |= GPIO_AFRL_AFSEL4_0;
		GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL4_1;
		GPIOB->AFR[0] |= GPIO_AFRL_AFSEL4_2;
		GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL4_3;
		
		GPIOB->AFR[0] |= GPIO_AFRL_AFSEL5_0;
		GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL5_1;
		GPIOB->AFR[0] |= GPIO_AFRL_AFSEL5_2;
		GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL5_3;
}


void SPI1_Init(void){
	// Enable SPI clock and Reset SPI
	
	RCC -> APB2ENR |= RCC_APB2ENR_SPI1EN;
	
	RCC -> APB2RSTR |= RCC_APB2RSTR_SPI1RST;
	RCC -> APB2RSTR &= ~RCC_APB2RSTR_SPI1RST;
	
	// Disable SPI
	
	SPI1 -> CR1 &= ~SPI_CR1_SPE;	
	
	// Configure for Full Duplex Communication
	
	SPI1 -> CR1 &= ~SPI_CR1_RXONLY;	
	
	// Configure for 2-line Unidirectional Data Mode
	
	SPI1 -> CR1 &= ~SPI_CR1_BIDIMODE;
	
	// Disable Output in Bidirectional Mode
	
	SPI1 -> CR1 &= ~SPI_CR1_BIDIOE;
	
	// Set Frame Format: MSB First, 16-bit, Motorola Mode
	
	SPI1 -> CR1 &= ~SPI_CR1_LSBFIRST;
	SPI1 -> CR2 &= ~SPI_CR2_FRF;
	
	SPI1 -> CR2 |= SPI_CR2_DS;                        
	
	
	// Configure Clock. Read DataSheet for required value
	
	SPI1 -> CR1 |= SPI_CR1_CPHA;
	SPI1 -> CR1 |= SPI_CR1_CPOL;
	
	// Set Baud Rate Prescaler to 16
	
	SPI1 -> CR1 |= SPI_CR1_BR_0;
	SPI1 -> CR1 |= SPI_CR1_BR_1;
	SPI1 -> CR1 &= ~SPI_CR1_BR_2;
	
	// Disable Hardware CRC Calculation
	
	SPI1 -> CR1 &= ~SPI_CR1_CRCEN;
	
	// Set as Master
	
	SPI1 -> CR1 |= SPI_CR1_MSTR;
	
	// Disable Software Slave Management
	
	SPI1 -> CR1 &= ~SPI_CR1_SSM;
	
	// Enable NSS Pulse Management
	
	SPI1 -> CR2 |= SPI_CR2_NSSP; 
	
	// Enable Output
	
	SPI1 -> CR2 |= SPI_CR2_SSOE;
	
	// Set FIFO Reception Threshold to 1/2
	
	SPI1 -> CR2 &= ~SPI_CR2_FRXTH;
	
	// Enable SPI
	
	SPI1 -> CR1 |= SPI_CR1_SPE;
}

uint16_t SPI_Transfer_Data(uint16_t write_data){

	uint16_t read_data = 0;
	
	// Wait for TXE (Transmit buffer empty)	
	while((SPI1 -> SR & SPI_SR_TXE) == 0);

	// Write data	
	//SPI1 -> DR &= ~SPI_DR_DR;
	SPI1 -> DR = write_data;
	
	// Wait for not busy
	while((SPI1 -> SR & SPI_SR_BSY) == SPI_SR_BSY);
	
	// Wait for RXNE (Receive buffer not empty)
	while((SPI1 -> SR & SPI_SR_RXNE) == 0);

	read_data = SPI1 -> DR;
	// Read data
	
	
	return read_data; // TODO
}