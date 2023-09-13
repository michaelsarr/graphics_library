

/*
 * Copyright (c) 2023 Rumbledethumps
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <rp6502.h>
#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define WIDTH 320
#define HEIGHT 240

#include "draw.h"



// i just started editing the brot32.c file

// This version optimized for fixed point math on 8-bit processors
typedef int32_t fint32_t;


#define VIDEO_MODE 1    // vga_320_240 ,76800 pixels  0x0000 to 0x32000

//#define VIDEO_MODE 2    // vga_320_180  ,57600 pixels

//#define VIDEO_MODE 3   // vga_640_480  ,307200 pixels
//#define VIDEO_MODE 4   // vga_640_360   ,230400 pixels

static void vmode(uint16_t data)
{
    xreg(data, 0, 1);
}

static void erase()
{
    // Partially unrolled loop is FAST
    unsigned i = 0;
    RIA_ADDR0 = 0;
    RIA_STEP0 = 1;
    for (i = 0x1300; --i;)
    {
        RIA_RW0 = 255;
        RIA_RW0 = 255;
        RIA_RW0 = 255;
        RIA_RW0 = 255;
        RIA_RW0 = 255;
        RIA_RW0 = 255;
        RIA_RW0 = 255;
        RIA_RW0 = 255;
    }
    RIA_ADDR0 = 0;
    for (i = 0x1300; --i;)
    {
        RIA_RW0  = 0;
        RIA_RW0 = 0;
        RIA_RW0 = 0;
        RIA_RW0 = 0;
        RIA_RW0 = 0;
        RIA_RW0 = 0;
        RIA_RW0 = 0;
        RIA_RW0 = 0;
    }
    RIA_ADDR0 = 0;
}

static void wait()
{

    uint8_t discard;
    discard = RIA_RX;
    while (RIA_RX_READY)
        discard = RIA_RX;
    while (!(RIA_RX_READY))
        ;
    discard = RIA_RX;
}
//-----------------------------------------------------
#define FAST
#define ball_count 25
#define ball_radius 7
uint16_t loop,loop2;
void bouncing_ball(uint16_t count,uint8_t erase)
{
uint16_t xvel[ball_count];
uint16_t yvel[ball_count];
uint16_t x[ball_count];
uint16_t y[ball_count];
uint8_t color[ball_count];


for(loop=0;loop<ball_count;++loop)
{
    xvel[loop] = 1+rand16()%5;
    yvel[loop] = 1+rand16()%5;
    x[loop] = rand16()%300;
    y[loop] = rand16()%16;
    color[loop] = rand16()%15;
}
while (count)
{
for(loop=0;loop < ball_count;++loop)
  {
  count--;
  if (erase)
  #ifdef FAST
      plot_circle(x[loop],y[loop],ball_radius,0);
  #else
    plot_circle_original(x[loop],y[loop],ball_radius,0);
  #endif
   
  x[loop]+= xvel[loop];
  y[loop]+= yvel[loop];
  #ifdef FAST
      plot_circle(x[loop],y[loop],ball_radius,color[loop]);
  #else
    plot_circle_original(x[loop],y[loop],ball_radius,color[loop]);
  #endif


  if (y[loop] >= HEIGHT -5)
    yvel[loop] = -1 * (rand16() %5);
  if (y[loop] <= 5)
    yvel[loop] =  rand16() %5;
  if (x[loop] >= WIDTH -5)
    xvel[loop] = -1 * (rand16() %5);
  if (x[loop] <= 5)
    xvel[loop] = rand16() %5;
  }
}


}
void colorfill(uint8_t color);

void ballfill(uint16_t count)
{
for(loop=0;loop<count;++loop)
#ifdef FAST
 plot_circle(rand16()%319,rand16()%240,ball_radius,rand16()%15);
#else
plot_circle_original(rand16()%319,rand16()%240,ball_radius,rand16()%15);
#endif
}
void linefill(uint16_t count)
{
for(loop=0;loop<count;++loop)
#ifdef FAST
 plot_line(rand16()%319,rand16()%240,rand16()%319,rand16()%240,rand16()%15);
 #else
 plot_line_original(rand16()%319,rand16()%240,rand16()%319,rand16()%240,rand16()%15);
 #endif

}
void sleep(uint16_t t)
{
while(t)
{printf(" ");--t; }
}


void main()
{

vmode(VIDEO_MODE);
erase();
RIA_STEP0 =0;


while(1)
{
bouncing_ball(30000,1);

  colorfill(1);
bouncing_ball(30000,1);
ballfill(7000);
bouncing_ball(30000,1);
linefill(3000);
bouncing_ball(30000,1);
bouncing_ball(30000,0);
}


}// end main

void colorfill(uint8_t color)
{
 for (loop =0;loop<WIDTH;loop++)
    {
    #ifdef FAST
    plot_line(320-loop,239,loop,0,color);
#else
    plot_line_original(320-loop,239,loop,0,color);
#endif
     //       if (loop % 20 ==1)
            color ++;
            color %= 16;
    //printf("a loop: %d\n",loop);
    //sleep(10);
    }     


    for (loop =0;loop<HEIGHT;loop++)
    {
            #ifdef FAST
        plot_line(319,loop,0,HEIGHT-loop,color);
        #else
               plot_line_original(319,loop,0,HEIGHT-loop,color); 
        #endif

     //   if (loop % 20 ==1)
        color ++;
        color %= 16;
    //        printf("b loop: %d\n",loop);
    //        sleep(10);
    }     



}

