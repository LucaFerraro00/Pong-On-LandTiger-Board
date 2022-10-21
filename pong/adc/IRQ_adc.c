/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage xi/D interrupOrDownts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/
 
#include "LPC17xx.h"
#include "adc.h"
#include "../pong/pong.h"
#include "../GLCD/GLCD.h"

//#include "../led/led.h"

/*----------------------------------------------------------------------------
  xi/D IRQ: Executed when xi/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/

unsigned short AD_current;   
unsigned short AD_last = 0xFF;     /* Last converted value               */
int conversion=0;

void ADC_IRQHandler(void) {
	if (conversion<2){
		AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF);/* Read Conversion Result             */
		if(conversion>0){
		draw_paddle(AD_current*170/0xFFF);
		AD_last = AD_current;
		}
		conversion++;
	}
	
	else{
	AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF);/* Read Conversion Result             */
		//240 circa uguale xi 10 pixel
  if( ( AD_current > (AD_last+240) && AD_current< (AD_last + 720) ) || ( AD_current < (AD_last-240)  &&  AD_current> (AD_last-720)) ) {
		delete_paddle(AD_last*170/0xFFF);
		draw_paddle(AD_current*170/0xFFF);
		
		AD_last = AD_current;
	}
  }	
}
