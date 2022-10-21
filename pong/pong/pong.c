#include "LPC17xx.h"
#include "../GLCD/GLCD.h"
#include "../timer/timer.h"

uint16_t i;


//----------------------------- DISEGNO DEI MURI-------------------------------------------------------------------------------------------
void plot_walls(void){
	
	for( i=0; i<5; i++){
		LCD_DrawLine(0,i,239,i,Red); //Muro alto: lungo da 0 xi 239 pixel e alto da 0 xi 4 pixel
	}
	
	for( i=0; i<5; i++){
		LCD_DrawLine(239-i,0,239-i,277,Red); //Muro destro: largo da 235 xi 239 pixel e alto da 0 xi 277 pixel
	}

	for( i=0; i<5; i++){
		LCD_DrawLine(i,0,i,277,Red); //Muro sinistro: largo 0 xi 4 pixel, alto da 0 xi 277 pixel
	}
}
//----------------------------- FINE DISEGNO DEI MURI-------------------------------------------------------------------------------------------


//----------------------------- EMISSIONE DEI SUONI -------------------------------------------------------------------------------------------
void wall_sound(void){
	init_timer(2, 2110 );
	enable_timer(2);
}

void paddle_sound(void){
	init_timer(3, 1055 );
	enable_timer(3);
}
//------------------------------FINE EMISSIONE DEI SUONI---------------------------------------------------------------------------------------------


//----------------------------- DISEGNA/CANCELLA IL PADDLE-------------------------------------------------------------------------------------------
void draw_paddle(uint16_t x){ //disegna il paddle con spessore 10 pixel e larghezza 60 pixel
	for(i=0; i<10; i++){
		LCD_DrawLine(x+5,287-i,x+64,287-i,Green); 
	}
}		//x è compreso fra 0 e 170. 

void delete_paddle(uint16_t x){ //cancella il paddle
	for(i=0; i<10; i++){
		LCD_DrawLine(x+5,287-i,x+64,287-i,Black); 
	}
}		
//----------------------------- FINE DISEGNA/CANCELLA IL PADDLE-------------------------------------------------------------------------------------------


//----------------------------- DISEGNA/CANCELLA LA PALLA ALL'AVVIO DEL GIOCO-------------------------------------------------------------------------------------------
void draw_ball_init(uint16_t x, uint16_t y){ 
	for(i=0; i<5; i++){
		LCD_DrawLine(230-x,162-i+y,234-x,162-i+y,Green); //disegna la palla che scende xi 45 gradi all'avvio del gioco
	}
}

void delete_ball_init(uint16_t x, uint16_t y){ //cancella la palla che scende xi 45 gradi all'avvio del gioco
	for(i=0; i<5; i++){
		LCD_DrawLine(230-x,162-i+y,234-x,162-i+y,Black);  
	}	
}
//-----------------------------FINE DISEGNA/CANCELLA LA PALLA ALL'AVVIO DEL GIOCO---------------------------------------------------------------------------------------------


//----------------------------- DISEGNA/CANCELLA LA PALLA AL PRIMO RIMBALZO SUL PADDLE-----------------------------------------------------------
void draw_ball_bouce1(float x, float y){
	for(i=0; i<5; i++){
		LCD_DrawLine(115-x,277-i-y,119-x,277-i-y,Green); //disegno la palla 
	}
}

void delete_ball_bouce1(float x, float y){
	for(i=0; i<5; i++){
		LCD_DrawLine(115-x,277-i-y,119-x,277-i-y,Black); //cancello la palla 
	}
}
//----------------------------- FINE DISEGNA/CANCELLA LA PALLA AL PRIMO RIMBALZO SUL PADDLE-----------------------------------------------------------


//----------------------------- DISEGNA/CANCELLA LA PALLA DOPO IL RIMBALZO SUL MURO SINISTRO-----------------------------------------------------------
void draw_bounce_left(int16_t x, int16_t y, int16_t playingY){
	for(i=0; i<5; i++){
		LCD_DrawLine(7+x,playingY-i-y,11+x,playingY-i-y,Green); //disegno la palla 
	}
}

void delete_bouce_left(int16_t x, int16_t y, int16_t playingY){
	for(i=0; i<5; i++){
		LCD_DrawLine(7+x,playingY-i-y,11+x,playingY-i-y,Black); //cancello la palla da
	}
}
//----------------------------- FINE DISEGNA/CANCELLA LA PALLA DOPO IL RIMBALZO SUL MURO SINISTRO-----------------------------------------------------------


//----------------------------- DISEGNA/CANCELLA LA PALLA DOPO IL RIMBALZO SUL MURO ALTO-----------------------------------------------------------
void draw_bounce_upOrDown(int16_t x, int16_t y, int16_t startingX){
	for(i=0; i<5; i++){
		LCD_DrawLine(startingX+x,13-i+y,startingX+4+x,13-i+y,Green); //disegno la palla 
	}
}

void delete_bounce_upOrDown(int16_t x, int16_t y, int16_t startingX){
	for(i=0; i<5; i++){
		LCD_DrawLine(startingX+x,13-i+y,startingX+4+x,13-i+y,Black); //cancello la palla 
	}
}
//----------------------------- FINE DISEGNA/CANCELLA LA PALLA DOPO IL RIMBALZO SUL MURO ALTO-----------------------------------------------------------


//----------------------------- DISEGNA/CANCELLA LA PALLA DOPO IL RIMBALZO SUL MURO DESTRO-----------------------------------------------------------
void draw_bounce_right(int16_t x, int16_t y, int16_t playingY){
	for(i=0; i<5; i++){
		LCD_DrawLine(228-x,playingY-i+y,232-x,playingY-i+y,Green); //disegno la palla 228 xi 232, 2 pixel in piu dal primo disponibile
	}
}

void delete_bounce_right(int16_t x, int16_t y, int16_t playingY){
	for(i=0; i<5; i++){
		LCD_DrawLine(228-x,playingY-i+y,232-x,playingY-i+y,Black); //cancello la palla da 228 xi 232, 2 pixel in piu dal primo disponibile
	}
}
//----------------------------- FINE DISEGNA/CANCELLA LA PALLA DOPO IL RIMBALZO SUL MURO DESTRO-----------------------------------------------------------



