#include "EXTI.h"

#include "DAC.h"

void EXTI_Init(void) {
    // Initialize User Button
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
		GPIOC->MODER &= ~(GPIO_MODER_MODE13); //Set PC13 to input
		GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD13); //Set PC13 to no pull up no pull down

		
		// Configure SYSCFG EXTI
		RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
		SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
		SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;

		
		// Configure EXTI Trigger
		EXTI->FTSR1 |= EXTI_FTSR1_FT13;
		
		// Enable EXTI
		EXTI->IMR1 |= EXTI_IMR1_IM13;
		
		// Configure and Enable in NVIC
		NVIC_EnableIRQ(EXTI15_10_IRQn);
		NVIC_SetPriority(EXTI15_10_IRQn, 0); 
}

#define DAC_MIN 0
#define DAC_MAX 4095
#define DAC_INCREMENT 512

int increasing = 0; 
int signed_DAC_value = 0; 
uint32_t DAC_value = 0; 

static uint32_t dac_value = 0;
static enum {
    DOWN,
    UP,
} direction = UP;

//Interrupt handler
void EXTI15_10_IRQHandler(void) {
	// Clear interrupt pending bit
	EXTI->PR1 |= EXTI_PR1_PIF13;
	// Define behavior that occurs when interrupt occurs
	if(increasing == 1)
	{
		DAC_value += DAC_INCREMENT; 
		signed_DAC_value += DAC_INCREMENT; 
		if(signed_DAC_value >= DAC_MAX)
		{
			signed_DAC_value = DAC_MAX;
			DAC_value = DAC_MAX;
			increasing = 0; 
		}
	}
	else if(increasing == 0)
	{
		DAC_value -= DAC_INCREMENT; 
		signed_DAC_value -= DAC_INCREMENT; 
		if(signed_DAC_value <= 0)
		{
			signed_DAC_value = 0;
			DAC_value = 0;
			increasing = 1; 
		}
	}
	DAC_Write_Value(DAC_value); 
}