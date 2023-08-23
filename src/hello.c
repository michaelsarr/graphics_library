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



void main()
{
int16_t loop=0;
int16_t loop2=0;
uint8_t color=2;
RIA_STEP0 = 0;
vmode(1);

   // wait();
    erase();
    RIA_ADDR0 = 0;
    RIA_STEP0 = 1;

    
 //   for ( color =1 ; color < 16;color ++)    
 //       for (loop =0;loop<180;loop++)
 //                plot_line(loop,0,180-loop,220,(uint8_t) color);
 // plot_circle(100,100,50,1); 
  //raster_circle(200,200,25,1); 

 // plot_ellipse_rect ( 10, 30,  75,  30, 2);

     plot_line(5,2,5,100,(uint8_t) color);
         plot_line(6,2,6,100,(uint8_t) color);
         
for (loop =0;loop<180;loop++)
  {
  plot_line(loop,10,loop,50,(uint8_t) color);
  for (loop2 =0;loop2<1000;loop2++)
        puts(" ");
  
  }


     plot_circle(100,100,50,1); 


     plot_line(29,2,29,100,(uint8_t) color);     

     plot_line(0,180,100,180,(uint8_t) color); 
              while(1){     }
    }


