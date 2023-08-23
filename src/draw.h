#ifndef DRAW_H
#define DRAW_H
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include "rp6502.h"


#define WIDTH 320
#define HEIGHT 240

/*
need some type of bounds checking
or not?
*/
#define COLOR_MASK 0b1111
void setPixel(int16_t x, int16_t y, uint8_t color);
/* functions below from 
https://gist.github.com/bert/1085538
*/


void plot_line(int16_t x0, int16_t y0, int16_t  x1, int16_t y1,uint8_t color);

void plot_circle (int16_t xm, int16_t ym, int16_t r,uint8_t color);

void raster_circle (int16_t x0, int16_t y0, int16_t radius, uint8_t color);

void plot_ellipse_rect (int x0, int y0, int x1, int y1,uint8_t color);

void plot_basic_bezier (int x0, int y0, int x1, int y1, int x2, int y2,uint8_t color);

#endif




