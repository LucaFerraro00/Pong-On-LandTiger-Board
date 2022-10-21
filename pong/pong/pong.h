#include "LPC17xx.h"
#include <string.h>

void plot_walls(void);
void draw_paddle(uint16_t x);
void delete_paddle(uint16_t x);

void delete_ball_init(uint16_t x, uint16_t y);
void draw_ball_init(uint16_t x, uint16_t y);
void draw_ball_bouce1(float x, float y);
void delete_ball_bouce1(float x, float y);
void delete_bouce_left(int16_t x, int16_t y, int16_t playingY);	
void draw_bounce_left(int16_t x, int16_t y, int16_t playingY);
void delete_bounce_upOrDown(int16_t x, int16_t y, int16_t playingY);	
void draw_bounce_upOrDown(int16_t x, int16_t y, int16_t playingY);
void delete_bounce_right(int16_t x, int16_t y, int16_t playingY);	
void draw_bounce_right(int16_t x, int16_t y, int16_t playingY);
void wall_sound(void);
void paddle_sound(void);
