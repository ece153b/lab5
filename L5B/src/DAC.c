#include "stm32l476xx.h"

static void DAC_Pin_Init(void) {
	//Setup PA4 for analog output
	//Enable clock for GPIO Port A
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	//Set mode to analog
	GPIOA->MODER |= GPIO_MODER_MODE4;
	
	//Set pull up/pull down to no pull up/no pull down
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD4);
}

void DAC_Write_Value(uint32_t value) {
    //Write DAC1Ch1 right-aligned 12-bit value
		DAC->DHR12R1 = (value); //TODO - make sure this is the right register
}

void DAC_Init(void) {
    DAC_Pin_Init();

    //Enable DAC clock
		RCC->APB1ENR1 |= RCC_APB1ENR1_DAC1EN;
	
		//Disable DAC
		DAC->CR &= ~(DAC_CR_EN1); 

    //Enable software trigger mode (Set TSEL1 to 111)
		DAC->CR |= DAC_CR_TSEL1; 

    //Disable trigger
		DAC->CR &= ~(DAC_CR_TEN1); 
	
    //DAC1 connected in normal mode to external pin only with buffer enabled (Set Mode1 to 000)
		DAC->MCR &= ~(DAC_MCR_MODE1); 

    //Enable DAC channel 1
		DAC->CR |= DAC_CR_EN1;
	
    DAC_Write_Value(0);
}
