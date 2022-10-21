/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupOrDownts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include "../pong/pong.h"
#include "../RIT/RIT.h"
#include <math.h>
#include <stdio.h>

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupOrDownt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
extern int into;
extern int conversion;
extern unsigned short AD_current;
extern uint8_t play;
extern int score;
char testo[4]="";
char testo1[4]="";

uint16_t SinTable[45] =                                      
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};

extern int record;
extern int cronologiaRecord[100];
extern int indice_record;

void TIMER0_IRQHandler (void)
{
	static int touchL=0;
	static int touchU=0;
	static int touchR=0;
	static int startingY_left=0; 
	static int startingY_right=0;
	static int startingX=0;
	
	static int xi,yi,x1,y1,xL,yL,xU,yU,xR,yR= 0; 
//xi e yi coordinata  da passare alla funzione draw/delete initial ball
//x1 e y1 coordinata  da passare alla funzione draw/delete ball al primo rimbalzo
//xL e yL coordinata  da passare alla funzione draw/delete ball che rimbalza sul muro sinistro (Left)
//xU e yU coordinata  da passare alla funzione draw/delete ball che rimbalza sul muro alto (Up)
//xR e yR coordinata  da passare alla funzione draw/delete ball che rimbalza sul muro destro (Right)
	
	static int rimbalzo_paddle=0;
	//rimablzo_paddle=0 -> la palla non ha ancora raggiunto l'altezza del paddle nella fase di avvio del gioco (prima discesa della palla)
	//rimablzo_paddle=1 -> la palla non ha ancora raggiunto l'altezza del paddle nella fase gioco in uno dei possibili rimablzi
	//rimablzo_paddle=2 -> la palla ha ancora raggiunto l'altezza del paddle, ma non lo ha "toccato" (Hai perso!)

	static int16_t direction = 0;
	static int16_t upOrDown = 0; //upOrDown=1 la palla deve essere disegnata verso l'alto; upOrDown=0  la palla deve essere disegnata verso il basso
	static int16_t rightOrLeft=0; //0 il rimbalzo proviene dal muro sinistro; 1 se il rimbalzo proviene dal muro destro
	static int hit=0; 
	int z=0;
		
	if(into==1){
		xi=yi=x1=y1=xL=yL=xU=yU=xR=yR = 0;
		into=0;
		touchL = 0;
		touchU = 0;
		touchR = 0;
		rimbalzo_paddle = 0;
		direction = 0;
		conversion=0;
		score=0;
	}

	if(play==1){
		
		if(rimbalzo_paddle == 0){
			//gestione palla che scende a 45 gradi
				delete_ball_init(xi,yi);
		
				xi = xi + 1;
				yi = yi + 1;
				draw_ball_init(xi,yi);
			
				if(yi>=115){
					if( xi>=((AD_current*170/0xFFF)-4) && xi<=((AD_current*170/0xFFF)+63) ){
						rimbalzo_paddle=1;
						upOrDown = 1;
						if(score<100){
							score=score+5;
						}
						else {score=score+10;}
						sprintf(testo,"%d",score );
						for ( z=0; z<10; z++){
							LCD_DrawLine(5,160-z,15,160-z,Black);
						}
						GUI_Text(5,160,(uint8_t *)testo ,White, Black);
						if(xi>=((AD_current*170/0xFFF)-4) && xi<((AD_current*170/0xFFF)+13)){
									direction = 1;
							}else if(xi>=((AD_current*170/0xFFF)+13) && xi<((AD_current*170/0xFFF)+30)){
								direction = 2;
								}else if(xi>=((AD_current*170/0xFFF)+30) && xi<((AD_current*170/0xFFF)+47)){
									direction = 3;
								}else if(xi>=((AD_current*170/0xFFF)+47) && xi<=((AD_current*170/0xFFF)+63)){
									direction = 4;
								}
							paddle_sound();
				}else
					{//la palla non ha "toccato" il paddle
					rimbalzo_paddle = 2;
				}
				}
				else 
				{
						rimbalzo_paddle = 0; 
					}
			
		}else if(rimbalzo_paddle == 1){			
			
//**************************************INIZIO GESTIONE MOVIMENTO PALLA PRIMO RIMBALZO SUL PADDLE**********************************************************
						if(touchL == 0 && touchU == 0 && touchR == 0){
							delete_ball_bouce1(x1,y1);
							switch(direction){
								case 1:
									x1 = x1 + 4;
									y1 = y1 + 1;
									break;
								case 2:
									x1 = x1 + 1;
									y1 = y1 + 2;
									break;
								case 3:
									x1 = x1 - 1;
									y1 = y1 + 2;
									break;
								case 4:
									x1 = x1 - 4;
									y1 = y1 + 1;
									break;
								default:
									break;
							}
							
							/* se la palla passa sopra il punteggio ridisegno il punteggio */							
							if(x1>=95 && x1<=110 && y1>= 113 && y1 < 127){
									sprintf(testo,"%d",score );
									for ( z=0; z<10; z++){
										LCD_DrawLine(5,160-z,15,160-z,Black);
									}
									GUI_Text(5,160,(uint8_t *)testo ,White, Black);	
								}
							
							draw_ball_bouce1(x1,y1);
								
							if(x1 < 108){//CONTROLLO SE LA PALLA HA TOCCATO IL MURO DESTRO
								touchL =0;
							} else{
								touchL = 1;
								wall_sound();
								startingY_left = 277-y1;
								delete_ball_bouce1(x1,y1);
							}
							if(x1 < -108){//CONTROLLO SE LA PALLA HA TOCCATO IL MURO SINISTRO
								touchR = 1;
								wall_sound();
								startingY_right = 277-y1;
								delete_ball_bouce1(x1,y1);
							}else{
								touchR = 0;
							}
							//IN QUESTA FASE NON SERVE IL CONTROLLO SUL TOCCO DEL MURO ALTRO
							
//**************************************FINE GESTIONE MOVIMENTO PALLA PRIMO RIMBALZO SUL PADDLE**********************************************************

							
							
//*********************************INIZIO GESTIONE MOVIMENTO PALLA CHE RIMBALZA SUL MURO SINISTRO***********************************************
						}else if(touchL == 1 && touchU == 0 && touchR == 0){		
							
							if(startingY_left - yL < 12){
								touchU = 1;
								wall_sound();
								delete_bouce_left(xL,yL,startingY_left);
								startingX = 8+xL;
								xU=0;
								yU=0;
								upOrDown = 0;
								rightOrLeft=0;
								hit=0;
								touchL = 0;
								touchR = 0;
							}else if (touchR==0){
								touchU = 0;
								touchL = 1;
								touchR = 0;
							}
							
							delete_bouce_left(xL,yL,startingY_left);
							if(upOrDown == 1){
								switch(direction){
									case 1:
										xL = xL + 4;
										yL = yL + 1;
										break;
									case 2:
										xL = xL + 1;
										yL = yL + 2;
										break;
									case 3:
										xL = xL + 1;
										yL = yL + 2;
										break;
									case 4:
										xL = xL + 4;
										yL = yL + 1;
										break;
									default:
										break;
								}
								
							}else{
									switch(direction){
									case 1:
										xL = xL + 4;
										yL = yL - 1;
										break;
									case 2:
										xL = xL + 1;
										yL = yL - 2;
										break;
									case 3:
										xL = xL + 1;
										yL = yL - 2;
										break;
									case 4:
										xL = xL + 4;
										yL = yL - 1;
										break;
									default:
										break;
								}								
							}
							
							/* se la palla passa sopra il punteggio ridisegno il punteggio */		
							if(xL>=0 && xL<=20 && (startingY_left - yL) >= 145 && (startingY_left - yL) <= 175){
									sprintf(testo,"%d",score );
									for ( z=0; z<10; z++){
										LCD_DrawLine(5,160-z,15,160-z,Black);
									}
									GUI_Text(5,160,(uint8_t *)testo ,White, Black);	
								}
							
						/* se la palla passa sopra il record ridisegno il record */		
							if(xL>=200 && xL<=230 && (startingY_left - yL) >= 3 && (startingY_left - yL) <= 28){
								sprintf(testo1,"%d",record);
									for ( z=0; z<10; z++){
										LCD_DrawLine(210,14-z,234,14-z,Black);
										}
									GUI_Text(210,5,(uint8_t *) testo1 ,White, Black);	
								}
							
							draw_bounce_left(xL,yL,startingY_left);
								
							if(xL <= 214 ){//CONTROLLO SE LA PALLA HA TOCCATO IL MURO DESTRO
								touchU = 0;
								touchL = 1;
								touchR = 0;
							}else{
								touchU = 0;
								touchL = 0;
								touchR = 1;
								wall_sound();
								delete_bouce_left(xL,yL,startingY_left);
								xR=0;
								yR=0;
								startingY_right = startingY_left - yL - 2;
							}
							
							if(startingY_left-yL >= 276){//CONTROLLO SE LA PALLA HA TOCCATO IL MURO SINISTRO
								if( xL>=((AD_current*170/0xFFF)-4) && xL<=((AD_current*170/0xFFF)+63) ){
									rimbalzo_paddle=1;
									upOrDown = 1;
										if(score<100){
											score=score+5;
											}
										else {score=score+10;}
									
										sprintf(testo,"%d",score );
										for ( z=0; z<10; z++){
											LCD_DrawLine(5,160-z,15,160-z,Black);
										}	
										GUI_Text(5,160,(uint8_t *)testo ,White, Black);						
								if(xL>=((AD_current*170/0xFFF)-4) && xL<((AD_current*170/0xFFF)+13)){
										direction = 1;
									 }else if(xL>=((AD_current*170/0xFFF)+13) && xL<((AD_current*170/0xFFF)+30)){
										direction = 2;
										}else if(xL>=((AD_current*170/0xFFF)+30) && xL<((AD_current*170/0xFFF)+47)){
											direction = 3;
											}else if(xL>=((AD_current*170/0xFFF)+47) && xL<=((AD_current*170/0xFFF)+63)){
									direction = 4;
									}
									paddle_sound();
								}else{
									rimbalzo_paddle = 2;
								}}
							}
							
//*********************************FINE GESTIONE MOVIMENTO PALLA CHE RIMBALZA SUL MURO SINISTRO***********************************************
					
								
//*********************************INIZIO GESTIONE MOVIMENTO PALLA CHE RIMBALZA SUL MURO DESTRO***********************************************
						else if(touchL == 0 && touchU == 0 && touchR == 1){
								delete_bounce_right(xR,yR,startingY_right);
								
								if(upOrDown == 1){
									switch(direction){
										case 1:
											xR = xR + 4;
											yR = yR - 1;
											break;
										case 2:
											xR = xR + 1;
											yR = yR - 2;
											break;
										case 3:
											xR = xR + 1;
											yR = yR - 2;
											break;
										case 4:
											xR = xR + 4;
											yR = yR - 1;
											break;
										default:
											break;
										}
									}else {//upOrDown=0{
											switch(direction){
											case 1:
												xR = xR + 4;
												yR = yR + 1;
												break;
											case 2:
												xR = xR + 1;
												yR = yR + 2;
												break;
											case 3:
												xR = xR + 1;
												yR = yR + 2;
												break;
											case 4:
												xR = xR + 4;
												yR = yR + 1;
												break;
											default:
												break;
										}							
									}
								/* se la palla passa sopra il punteggio ridisegno il punteggio */		
								if(xR>=200 && xR<=225 && (startingY_right + yR) >= 145 && (startingY_right + yR) <= 165 ){
									sprintf(testo,"%d",score );
									for ( z=0; z<10; z++){
										LCD_DrawLine(5,160-z,15,160-z,Black);
										}
									GUI_Text(5,160,(uint8_t *)testo ,White, Black);	
									}
								
							/* se la palla passa sopra il record ridisegno il record */		
								if(xR>=0 && xR<=30 && (startingY_right + yR) >= 3 && (startingY_right + yR) <= 33 ){
									sprintf(testo1,"%d",record);
									for ( z=0; z<10; z++){
										LCD_DrawLine(210,14-z,234,14-z,Black);
										}
									GUI_Text(210,5,(uint8_t *) testo1 ,White, Black);	
									}
								
								draw_bounce_right(xR,yR,startingY_right);						
								
								if(xR < 220){
									touchR = 1;
									touchL = 0;
									touchU = 0;
								}else{ //la palla ha toccato il muro sinistro{
									touchL = 1;
									wall_sound();
									delete_bounce_right(xR,yR,startingY_right);
									xL = 0;
									yL = 0;
									touchR = 0;
									touchU = 0;
									startingY_left = startingY_right + yR + 2;
								}
								
								if(startingY_right + yR < 15){
									//la palla ha toccato il muro alto
									touchU = 1;
									wall_sound();
									delete_bounce_right(xR,yR,startingY_right);
									startingX=228-xR;
									rightOrLeft=1;
									upOrDown = 0;
									xU=0;
									yU=0;
									touchL = 0;
									touchR = 0;
									hit=0;
									}else if(touchL==0){
										touchU = 0;
										touchL = 0;
										touchR = 1;
									}
									
								
							if(startingY_right+yR>=276){//CONTROLLO SE LA PALLA TOCCA IL PADDLE
								if((228-xR)>=((AD_current*170/0xFFF)-4) && (228-xR)<=((AD_current*170/0xFFF)+63)){
									rimbalzo_paddle=1;
									upOrDown = 1;
										if(score<100){
											score=score+5;
											}
									else {score=score+10;}
									
									sprintf(testo,"%d",score );
									for ( z=0; z<10; z++){
										LCD_DrawLine(5,160-z,15,160-z,Black);
									}
									GUI_Text(5,160,(uint8_t *)testo ,White, Black);
									
									if((228-xR)>=((AD_current*170/0xFFF)-4) && (228-xR)<((AD_current*170/0xFFF)+13)){
												direction = 1;
										}else if((228-xR)>=((AD_current*170/0xFFF)+13) && (228-xR)<((AD_current*170/0xFFF)+30)){
											direction = 2;
											}else if((228-xR)>=((AD_current*170/0xFFF)+30) && (228-xR)<((AD_current*170/0xFFF)+47)){
												direction = 3;
											}else if((228-xR)>=((AD_current*170/0xFFF)+47) && (228-xR)<=((AD_current*170/0xFFF)+63)){
												direction = 4;
											}
											paddle_sound();
								}else{
									rimbalzo_paddle = 2;
								}}
							}
						
						
//*********************************FINE GESTIONE MOVIMENTO PALLA CHE RIMBALZA SUL MURO DESTRO***********************************************
						
							
//*********************************INIZIO GESTIONE MOVIMENTO PALLA CHE RIMBALZA SUL MURO ALTO***********************************************
						else if(touchL == 0 && touchU == 1 && touchR == 0){
							delete_bounce_upOrDown(xU,yU,startingX);
							
							if(hit==0){ //la palla è diretta verso il basso
								if(rightOrLeft==0){//la palla arriva da sinistra
									switch(direction){
											case 1:
												xU = xU + 4;
												yU = yU + 1;
												break;
											case 2:
												xU = xU + 1;
												yU = yU + 2;
												break;
											case 3:
												xU = xU + 1;
												yU = yU + 2;
												break;
											case 4:
												xU = xU + 4;
												yU = yU + 1;
												break;
											default:
												break;
										}
									
										if(startingX + xU < 230){
											touchU = 1;
											touchL = 0;
											touchR = 0;
										}else{
											touchU = 0;
											touchL = 0;
											touchR = 1;
											wall_sound();
											delete_bounce_upOrDown(xU,yU,startingX);
											startingY_right = 13+yU;
											xR=0;
											yR=0;
											upOrDown=0;
										}
									}
								else{//la palla arriva da destra
									switch(direction){
											case 1:
												xU = xU - 4;
												yU = yU + 1;
												break;
											case 2:
												xU = xU - 1;
												yU = yU + 2;
												break;
											case 3:
												xU = xU - 1;
												yU = yU + 2;
												break;
											case 4:
												xU = xU - 4;
												yU = yU + 1;
												break;
											default:
												break;
										}
										if(startingX + xU > 9){
											touchU = 1;
											touchL = 0;
											touchR = 0;
										}else{
											touchU = 0;
											touchL = 1;
											wall_sound();
											touchR = 0;
											delete_bounce_upOrDown(xU,yU,startingX);
											startingY_left = 15+yU;
											xL=0;
											yL=0;
											upOrDown=0;
										}
								}
						}// hit=1 la palla ha colpito il padlle, deve essere diretta verso l'alto
						else{
							if(rightOrLeft==0){// la palla arriva proviene da sinistra
								switch(direction){
										case 1:
											xU = xU + 4;
											yU = yU - 1;
											break;
										case 2:
											xU = xU + 1;
											yU = yU - 2;
											break;
										case 3:
											xU = xU + 1;
											yU = yU - 2;
											break;
										case 4:
											xU = xU + 4;
											yU = yU - 1;
											break;
										default:
											break;
									}
								
									if(startingX + xU < 230){
										touchU = 1;
										touchL = 0;
										touchR = 0;
									}else{
										touchU = 0;
										touchL = 0;
										touchR = 1;
										wall_sound();
										delete_bounce_upOrDown(xU,yU,startingX);
										startingY_right = 13+yU;
										xR=0;
										yR=0;
										upOrDown=1;
									}
								}
							else{//la palla arriva da destra
								switch(direction){
										case 1:
											xU = xU - 4;
											yU = yU - 1;
											break;
										case 2:
											xU = xU - 1;
											yU = yU - 2;
											break;
										case 3:
											xU = xU - 1;
											yU = yU - 2;
											break;
										case 4:
											xU = xU - 4;
											yU = yU - 1;
											break;
										default:
											break;
									}
								
								if(startingX + xU > 7){
										touchU = 1;
										touchL = 0;
										touchR = 0;
									}else{
										touchU = 0;
										touchL = 1;
										wall_sound();
										delete_bounce_upOrDown(xU,yU,startingX);
										touchR = 0;
										startingY_left = 13+yU;
										xL=0;
										yL=0;
										upOrDown=1;
										hit=0;
									}
							}
						
						}
						
						/* se la palla passa sopra il punteggio ridisegno il punteggio */		
							if((startingX+xU)>=3 && (startingX+xU)<=30 && yU >= 125 && yU <= 155 ){
								sprintf(testo,"%d",score );
								for ( z=0; z<10; z++){
									LCD_DrawLine(5,160-z,15,160-z,Black);
									}
								GUI_Text(5,160,(uint8_t *)testo ,White, Black);	
								}
						
						/* se la palla passa sopra il record ridisegno il record */		
							if((startingX+xU)>=215 && (startingX+xU)<=240 && yU >= -8 && yU <= 20 ){
								sprintf(testo1,"%d",record);
									for ( z=0; z<10; z++){
										LCD_DrawLine(210,14-z,234,14-z,Black);
										}
									GUI_Text(210,5,(uint8_t *) testo1 ,White, Black);
								}
								
							if (touchU==1){
							draw_bounce_upOrDown(xU,yU,startingX);
							}
															
							if(15+yU>=276){//CONTROLLO SE LA PALLA TOCCA IL PADDLE
								if( (startingX+xU)>=((AD_current*170/0xFFF)-4) && (startingX+xU)<=((AD_current*170/0xFFF)+63) ){
									rimbalzo_paddle=1;
									hit = 1;
									upOrDown=1;
										if(score<100){
										score=score+5;
											}	
										else {score=score+10;}
										
										sprintf(testo,"%d",score );
										for ( z=0; z<10; z++){
											LCD_DrawLine(5,160-z,15,160-z,Black);
										}
										GUI_Text(5,160,(uint8_t *)testo ,White, Black);
										
									if((startingX+xU)>=((AD_current*170/0xFFF)-4) && (startingX+xU)<((AD_current*170/0xFFF)+13)){
												direction = 1;
										}else if((startingX+xU)>=((AD_current*170/0xFFF)+13) && (startingX+xU)<((AD_current*170/0xFFF)+30)){
											direction = 2;
											}else if((startingX+xU)>=((AD_current*170/0xFFF)+30) && (startingX+xU)<((AD_current*170/0xFFF)+47)){
												direction = 3;
											}else if((startingX+xU)>=((AD_current*170/0xFFF)+47) && (startingX+xU)<=((AD_current*170/0xFFF)+63)){
												direction = 4;
											}
											paddle_sound();
								}else{
									rimbalzo_paddle = 2;
								}}
							}		
							
							
//*********************************FINE GESTIONE MOVIMENTO PALLA CHE RIMBALZA SUL MURO ALTO***********************************************
								
						}else {
				GUI_Text(70,150,(uint8_t *)"You lose!" ,Red, Black);
				GUI_Text(40,180,(uint8_t *)"press INTO to " ,White, Black);
				GUI_Text(40,200,(uint8_t *)"start a new game " ,White, Black);
				disable_timer(0);
				disable_timer(1);
				if(score > record){
					record = score;
					cronologiaRecord[indice_record] = record;
					indice_record = (indice_record+1)%100;
				}
			}
		}
	
  LPC_TIM0->IR = 1;			/* clear interrupOrDownt flag */
  return;
}


void TIMER1_IRQHandler (void)
{
	ADC_start_conversion();
  LPC_TIM1->IR = 1;			/* clear interrupOrDownt flag */
  return;
}


void TIMER2_IRQHandler (void)
{
	static int ticks=0;	
	LPC_DAC->DACR = (SinTable[ticks%45])<<6;
	ticks++;
	if(ticks==90){
		disable_timer(2);
		ticks=0;
		LPC_DAC->DACR = 0;
	}
  LPC_TIM2->IR = 1;			/* clear interrupOrDownt flag */
  return;
}

void TIMER3_IRQHandler (void)
{
	static int ticks=0;
	LPC_DAC->DACR = (SinTable[ticks%45])<<6;
	ticks++;
	if(ticks==90){
		disable_timer(3);
		ticks=0;
		LPC_DAC->DACR = 0;
	}
  LPC_TIM3->IR = 1;			/* clear interrupOrDownt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
