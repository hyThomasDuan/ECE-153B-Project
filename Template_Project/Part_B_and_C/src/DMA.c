/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Project
 */
 
#include "DMA.h"
#include "UART.h"
#include "SysTimer.h"

void DMA_Init_UARTx(DMA_Channel_TypeDef * tx, USART_TypeDef * uart){
	
	  USART_Init(uart);
	
	 //uint32_t BuffAdd = (uint32_t)&active; 
	 //tx->CMAR = BuffAdd; //store data source address
	

	
		RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN; //Enable DMA clock
		delay(2);
		tx->CPAR = (uint32_t)&(uart->TDR);
		tx->CCR &= ~DMA_CCR_EN; //disable the channel
	  tx->CCR &= ~DMA_CCR_MEM2MEM; //memory-to-memory mode
	
		tx->CCR &= ~DMA_CCR_PL;
		tx->CCR |= DMA_CCR_PL_1; //high channel priority
	
		tx->CCR &= ~DMA_CCR_PSIZE;// 8-bit
	
		tx->CCR &= ~DMA_CCR_MSIZE;// 8-bit
	
		tx->CCR &= ~DMA_CCR_PINC; //disable peripheral increment mode
		tx->CCR |= DMA_CCR_MINC; //disable memory increment mode
		tx->CCR &= ~DMA_CCR_CIRC; //disable circular mode
		tx->CCR |= DMA_CCR_DIR; //read from memory = memory->peripheral		
				
		tx->CCR &= ~DMA_CCR_HTIE; //disable half transfer interrupt
		tx->CCR &= ~DMA_CCR_TEIE; //disable transfer error interrupt
		tx->CCR |= DMA_CCR_TCIE; //enable transfer complete interrupt

	  DMA1_CSELR -> CSELR = 0x02000000; ///

}
	
void Init_DMA1_Channel4_IRQn(void){ //USART1
		NVIC_SetPriority(DMA1_Channel4_IRQn, 0); // Set Priority to 0
		NVIC_EnableIRQ(DMA1_Channel4_IRQn); // enable
}

void Init_DMA1_Channel7_IRQn(void){ //USART2
		NVIC_SetPriority(DMA1_Channel7_IRQn, 1); // Set Priority to 0
		NVIC_EnableIRQ(DMA1_Channel7_IRQn); // enable
}

void DMA1_Channel4_IRQHandler(void){ 
		NVIC_ClearPendingIRQ(DMA1_Channel4_IRQn); //clear NVIC interrupt (pending) flag
		if((DMA1->ISR & DMA_ISR_TCIF4) == DMA_ISR_TCIF4){
				DMA1->ISR &= ~DMA_ISR_TCIF4;
				DMA1_Channel4->CCR &= ~DMA_CCR_EN; //disable DMA
			  clearActiveBuffer();
			  on_complete_transfer();
		}
		DMA1->IFCR |= DMA_IFCR_CGIF4; // clear global DMA interrupt flag
}

void DMA1_Channel7_IRQHandler(void){ 
	
		NVIC_ClearPendingIRQ(DMA1_Channel7_IRQn); //clear NVIC interrupt (pending) flag
		if((DMA1->ISR & DMA_ISR_TCIF7) == DMA_ISR_TCIF7){
				DMA1->ISR &= ~DMA_ISR_TCIF7;
				DMA1_Channel7->CCR &= ~DMA_CCR_EN; //disable DMA
			  clearActiveBuffer();
			  on_complete_transfer();
		}
		DMA1->IFCR |= DMA_IFCR_CGIF7; // clear global DMA interrupt flag

	}
