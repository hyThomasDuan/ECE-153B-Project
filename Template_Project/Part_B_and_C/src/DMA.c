/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Project
 */
 
#include "DMA.h"
#include "SysTimer.h"

void DMA_Init(void) {
	
		RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN; //Enable DMA clock
		delay(2);
		DMA1_Channel6->CCR &= ~DMA_CCR_EN; //disable the channel
		DMA1_Channel6->CCR |= DMA_CCR_MEM2MEM; //memory-to-memory mode
	
		DMA1_Channel6->CCR &= ~DMA_CCR_PL;
		DMA1_Channel6->CCR |= DMA_CCR_PL_1; //high channel priority
	
		DMA1_Channel6->CCR &= ~DMA_CCR_PSIZE;
		DMA1_Channel6->CCR |= DMA_CCR_PSIZE_1; //32-bit peripheral size
	
		DMA1_Channel6->CCR &= ~DMA_CCR_MSIZE;
		DMA1_Channel6->CCR |= DMA_CCR_MSIZE_1; //32-bit memory size
	
		DMA1_Channel6->CCR &= ~DMA_CCR_PINC; //disable peripheral increment mode
		DMA1_Channel6->CCR |= DMA_CCR_MINC; //disable memory increment mode
		DMA1_Channel6->CCR &= ~DMA_CCR_CIRC; //disable circular mode
		DMA1_Channel6->CCR |= DMA_CCR_DIR; //read from memory = memory->peripheral
	
		uint32_t BuffAdd = (uint32_t)&DataBuffer; 
		uint32_t CrcDrAdd = (uint32_t)&CRC->DR;
		DMA1_Channel6->CMAR = BuffAdd; //store data source address
		DMA1_Channel6->CPAR = CrcDrAdd; //store data destination address
		
				
		DMA1_Channel6->CCR &= ~DMA_CCR_HTIE; //disable half transfer interrupt
		DMA1_Channel6->CCR &= ~DMA_CCR_TEIE; //disable transfer error interrupt
		DMA1_Channel6->CCR |= DMA_CCR_TCIE; //enable transfer complete interrupt
	
		NVIC_SetPriority(DMA1_Channel6_IRQn, 0); // Set Priority to 0
		NVIC_EnableIRQ(DMA1_Channel6_IRQn); // enable
		
}

void DMA1_Channel6_IRQHandler(void){ 
		NVIC_ClearPendingIRQ(DMA1_Channel6_IRQn); //clear NVIC interrupt (pending) flag
		if((DMA1->ISR & DMA_ISR_TCIF6) == DMA_ISR_TCIF6){
				DMA1->ISR &= ~DMA_ISR_TCIF6;
				completeCRC((uint32_t)CRC->DR);
		}
		DMA1->IFCR |= DMA_IFCR_CGIF6; // clear global DMA interrupt flag
}
