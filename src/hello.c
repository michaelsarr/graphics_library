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
int16_t x,y,r,c;


vmode(1);


    erase();
    RIA_ADDR0 = 0;
    RIA_STEP0 = 0;// oh man i missed this for a while.. really throws off trying to write when the thing is incrmenting

    while(1)
    {
  
    for (loop =0;loop<WIDTH;loop++)

            plot_line(loop,0,WIDTH-1-loop,240,(loop / 15)+1);

    for (loop =0;loop<HEIGHT;loop++)
            plot_line(0,loop,WIDTH-1,HEIGHT-1-loop ,(loop / 15)+1);

    for (loop2=0;loop2<10;loop2++)
      {
      r=(rand16()%25) +5;    
      x=rand16()%(320-r);
      y=rand16()%(240-r);
      c=rand16()%16;
      for (loop =0;loop<r;loop++)
        raster_circle(x,y,loop,c); 
      }
    wait();
    }
    }


