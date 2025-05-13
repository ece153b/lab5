/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 5A
 */


#include "ADC.h"
#include "PWM.h"
#include "SysClock.h"
#include "stm32l476xx.h"
#include <stdio.h>

int main(void) {
    // Initialization
    System_Clock_Init(); // Switch System Clock = 16 MHz

    ADC_Init();

    // [TODO] Initialize PWM

    while (1) {
        //Trigger ADC and get result
				ADC1->CR |= ADC_CR_ADSTART;
				while(!(ADC123_COMMON->CSR &= ADC_CSR_EOC_MST)); //TODO: check this
				uint16_t ADC_reading = (ADC1->DR & ADC_DR_RDATA); //TODO: check this as well 
        // [TODO] LED behavior based on ADC result
    }
}
