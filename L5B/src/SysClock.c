#include "SysClock.h"

void System_Clock_Init(void) {
	// Select MSI as system clock source
	//Replace with value that will make MSI the system clock source - 00 for MSI
	RCC->CFGR &= ~RCC_CFGR_SW; 
	
	
	// Set MSI clock range
	RCC->CR &= ~RCC_CR_MSIRANGE;
	 //Replace with value that will make range 8 MHz - 0111: range 7 around 8 MHz
	RCC->CR |= RCC_CR_MSIRANGE_5; //Change this so that its 2 MHz
	
	// Use the MSI clock range that is defined in RCC_CR
	//Replace with value that will select range in RCC->CR
	RCC->CR |= RCC_CR_MSIRGSEL; 
	
	// Enable MSI oscillator
	//Replace with value that will enable MSI
	RCC->CR |= RCC_CR_MSION; 
	
	// Wait until MSI is ready
	 /*Replace with value that checks whether MSI is ready */
	while((RCC->CR & RCC_CR_MSIRDY) == 0);
}
