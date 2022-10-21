/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupOrDownts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../pong/pong.h"
#include "../GLCD/GLCD.h"
#include "../timer/timer.h"
#include "../ADC/adc.h"
#include <stdio.h>
/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRupOrDownT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
/**/

extern uint8_t pauseResume_current;

extern int down_INT0;
extern int down_KEY1;
extern int down_KEY2;
extern int conversion;
extern char testo[4];
extern char testo1[4];
extern int record;
extern short AD_current;
extern short AD_last;
extern uint8_t pauseResume_last;
extern uint8_t pauseResume_current;

void RIT_IRQHandler (void)
{						
	int z  = 0;
	/* button management */
		/* INT0 */
		if(down_INT0!=0){ 
			if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	/* INT0 pressed */
				down_INT0++;				
				switch(down_INT0){
					case 2:
						disable_timer(0);
						disable_timer(1);	
						LCD_Clear(White);
					GUI_Text(8,120,(uint8_t *)"KEY1 : start the game" , White, Black);
					GUI_Text(8,140,(uint8_t *)"KEY2 : stop/resume the game" , White, Black);
					GUI_Text(8,160,(uint8_t *)"INTO : restart the game" , White, Black);
						reset_RIT(); 
						
					break;
					default:
						break;
				}
			}
			else {	/* button released */
				down_INT0=0;			
				NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupOrDownts			*/
				LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupOrDownt 0 pin selection */
					disable_RIT();
				reset_RIT(); 
			}
		}
		/* KEY1 */
		if(down_KEY1!=0){ 
			if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){	/* KEY1 pressed */
				down_KEY1++;				
				switch(down_KEY1){
					case 2:
							LCD_Clear(Black);
							plot_walls();
							ADC_start_conversion();
							if (conversion!=0){
								draw_ball_init(0,0);
								delete_paddle(AD_last*170/0xFFF);
								draw_paddle(AD_current*170/0xFFF);
							}
							GUI_Text(5,160,(uint8_t *)"0" ,White, Black);
							sprintf(testo1,"%d",record);
									for ( z=0; z<10; z++){
										LCD_DrawLine(210,14-z,234,14-z,Black);
										}
									GUI_Text(210,5,(uint8_t *) testo1 ,White, Black);							
							enable_timer(0);
							enable_timer(1);
							reset_RIT(); 
						break;
					default:
						break;
				}
			}
			else {	/* button released */
				down_KEY1=0;			
				NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupOrDownts			*/
				LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupOrDownt 0 pin selection */
				disable_RIT();
				reset_RIT();
			}
		}
		/* KEY2 */
		if(down_KEY2!=0){ 
			if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){	/* KEY2 pressed */
				down_KEY2++;				
				switch(down_KEY2){
					case 2:
						if(pauseResume_last == 0){
							pauseResume_current = 1;
							pauseResume_last = pauseResume_current;
						}else{
							pauseResume_current = 0;
							pauseResume_last = pauseResume_current;			
						}
						if(pauseResume_current == 1){
							disable_timer(0);
							disable_timer(1);	
							reset_RIT(); 
						}else{
							enable_timer(0);
							enable_timer(1);
							reset_RIT(); 
						}
						break;
					default:
						break;
				}
			}
			else {	/* button released */
				down_KEY2=0;			
				NVIC_EnableIRQ(EINT2_IRQn);							 /* enable Button interrupOrDownts			*/
				LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupOrDownt 0 pin selection */
				disable_RIT();
				reset_RIT();
			}
		}
			
	reset_RIT();
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupOrDownt flag */
	
	return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
