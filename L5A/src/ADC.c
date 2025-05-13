#include "ADC.h"

#include "stm32l476xx.h"

#include <stdint.h>

void ADC_Wakeup(void) {
    int wait_time;

    // To start ADC operations, the following sequence should be applied
    // DEEPPWD = 0: ADC not in deep-power down
    // DEEPPWD = 1: ADC in deep-power-down (default reset state)
    if ((ADC1->CR & ADC_CR_DEEPPWD) == ADC_CR_DEEPPWD)
        ADC1->CR &= ~ADC_CR_DEEPPWD; // Exit deep power down mode if still in that state

    // Enable the ADC internal voltage regulator
    // Before performing any operation such as launching a calibration or enabling the ADC, the ADC
    // voltage regulator must first be enabled and the software must wait for the regulator start-up
    // time.
    ADC1->CR |= ADC_CR_ADVREGEN;

    // Wait for ADC voltage regulator start-up time
    // The software must wait for the startup time of the ADC voltage regulator (T_ADCVREG_STUP)
    // before launching a calibration or enabling the ADC.
    // T_ADCVREG_STUP = 20 us
    wait_time = 20 * (80000000 / 1000000);
    while (wait_time != 0) {
        wait_time--;
    }
}

void ADC_Common_Configuration() {
  //Enable I/O analog switch voltage booster
	SYSCFG->CFGR1 |= SYSCFG_CFGR1_BOOSTEN; 
	
	//Modify ADC common control register
	//Enable Vrefint
	ADC123_COMMON->CCR |= ADC_CCR_VREFEN; 
	//Ensure clock is not divided - set PRESC to 0000
	ADC123_COMMON->CCR &= ~(ADC_CCR_PRESC); 
	//Ensure that the ADC clock scheme is set to HCLK/1 synchronous clock mode (set CKMODE to 01)
	ADC123_COMMON->CCR &= ~(ADC_CCR_CKMODE); 
	ADC123_COMMON->CCR |= ADC_CCR_CKMODE_0; 
	//Ensure that all ADCs are operating in independent mode (set DUAL to 00000)
	ADC123_COMMON->CCR &= ~(ADC_CCR_DUAL); 
}

void ADC_Pin_Init(void) {
  //Setup PA1 for analog input
	//Enable clock for GPIO Port A
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	//Set mode to analog
	GPIOA->MODER |= GPIO_MODER_MODE1;
	
	//Set pull up/pull down to no pull up/no pull down
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD1);
	
	//Connect PA1 to ADC input
	GPIOA->ASCR |= GPIO_ASCR_ASC1; 
}

void ADC_Init(void) {
  //Enable & Reset ADC Clock
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN; 
	RCC->AHB2RSTR |= RCC_AHB2RSTR_ADCRST;
	RCC->AHB2RSTR &= ~(RCC_AHB2RSTR_ADCRST); 
	
	//Disable ADC
	ADC1->CR &= ~(ADC_CR_ADEN); 	

	// Other ADC Initialization
	ADC_Pin_Init();
	ADC_Common_Configuration();
	ADC_Wakeup();

  //Other Configuration
	//12-bit resolution (set RES to 00)
	ADC1->CFGR &= ~(ADC_CFGR_RES); 
	//Right alignment (set ALIGN to 0)
	ADC1->CFGR &= ~(ADC_CFGR_ALIGN); 
	//Set sequence length to 1 in regular sequence register
	ADC1->SQR1 &= ~(ADC_SQR1_L);
	//ensure that channel 6 is used for the first conversion
	ADC1->SQR1 |= (6<<6); //TODO: double check this
	//In the ADC differential mode selection register, ensure that channel 6 is set to single-ended mode
	ADC1->DIFSEL &= ~(ADC_DIFSEL_DIFSEL_6); 
	//In the ADC sample time register, set the sampling time for channel 6 to be 24.5 ADC clock cycles
	//(Set to 011 for 24.5)
	ADC1->SMPR1 &= ~(ADC_SMPR1_SMP6); 
	ADC1->SMPR1 |= ADC_SMPR1_SMP1; 
	ADC1->SMPR1 |= ADC_SMPR1_SMP2; 
	//In the ADC configuration register, ensure that the ADC is in single conversion mode and that hardware trigger detection is disabled
	//(Set CONT to 0 and EXTEN to 00)
	ADC1->CFGR &= ~(ADC_CFGR_CONT); 
	ADC1->CFGR &= ~(ADC_CFGR_EXTEN); 
	//Enable ADC
	ADC1->CR &= ~(ADC_CR_ADEN); 	
	//Wait until ADC is ready
	while (!(ADC1->ISR & ADC_ISR_ADRDY)); 
}
