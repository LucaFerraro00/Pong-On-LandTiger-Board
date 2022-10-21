#include "button.h"
#include "lpc17xx.h"
#include "../timer/timer.h"
#include "../pong/pong.h"
#include "../RIT/RIT.h"		  			


int down_INT0=0;
int down_KEY1=0;
int down_KEY2=0;

uint8_t pauseResume_last = 0;
uint8_t pauseResume_current = 0;
uint8_t play = 0;
int into=0; //quando into=1 devo ripristinare i valori iniziali di tutte le variabili usate dal timer 0


void EINT0_IRQHandler (void)	  	/* INT0														 */
{		
	NVIC_DisableIRQ(EINT0_IRQn);		/* disable Button interrupOrDownts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 20);     /* GPIO pin selection */
	down_INT0=1;
	play=0;
	into=1;
	enable_RIT();	
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupOrDownt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
	NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupOrDownts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
	down_KEY1=1;
	play=1;

	enable_RIT();	
	LPC_SC->EXTINT &= (1 << 1);  	/* clear pending interrupOrDownt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	NVIC_DisableIRQ(EINT2_IRQn);		/* disable Button interrupOrDownts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 24);     /* GPIO pin selection */
	down_KEY2=1;
	enable_RIT();	
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupOrDownt         */    
}

