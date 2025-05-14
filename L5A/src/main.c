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

uint16_t curr_reading; 
uint16_t prev_reading;

int main(void) {
    // Initialization
    System_Clock_Init(); // Switch System Clock = 16 MHz

    ADC_Init();

    // Initialize PWM
		LED_Pin_Init(); 
		TIM2_CH1_Init(); 
		
		TIM2->CCR1 = 0; 
		curr_reading = (ADC1->DR & ADC_DR_RDATA); 
		prev_reading = 0; 

		
    while (1) {
        //Trigger ADC and get result
				ADC1->CR |= ADC_CR_ADSTART;
				while(!(ADC123_COMMON->CSR &= ADC_CSR_EOC_MST)); //TODO: check this
				prev_reading = curr_reading; 
				curr_reading = (ADC1->DR & ADC_DR_RDATA); //TODO: check this as well 
        // [TODO] LED behavior based on ADC result
				TIM2->CCR1 = (curr_reading*TIM2->ARR)/0x0FFF; 
    }
		

}
