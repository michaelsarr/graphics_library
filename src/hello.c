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


void main()
{
int16_t loop=0;
int16_t loop2=10;
uint8_t color=1;

vmode(VIDEO_MODE);
erase();

#define FAST

#ifdef FAST




while(1)
{
uint16_t x = rand16() % 310;
uint16_t y = rand16() % 230;
uint16_t r = rand16() % 10;

//for (loop=1;loop<5;++loop)
 plot_circle(x,y,r,color);

color++;
if (color >15)
color=2;
//ait();
}


    while(1)
    {
    loop2--;
    for (loop =0;loop<WIDTH;loop++)
    {
            plot_line(160,120,loop,0,color);

            
            if (loop % 20 ==1)
            color ++;
            color %= 16;
    }     


    for (loop =0;loop<HEIGHT;loop++)
    {
        plot_line(319,loop,160,120,color);
        if (loop % 20 ==1)
        color ++;
        color %= 16;
    }     



    for (loop =WIDTH-1;loop>-1;loop--)
        {
       plot_line(160,120,loop,239,color);
        if (loop % 20 ==1)
        color ++;
        color %= 16;
        }     


    for (loop =HEIGHT-1;loop>-1;loop--)
    {
           plot_line(160,120,0,loop  ,color);
        if (loop % 20 ==1)
        color ++;
        color %= 16;
        }             
        color ++;
        color %= 16;

  
    }
#else
RIA_STEP0=0;
    while(1)
    {

    for (loop =0;loop<WIDTH;loop++)
    {
            plot_line_original(160,120,loop,0,color);
            if (loop % 20 ==1)
            color ++;
            color %= 16;
    }     

    for (loop =0;loop<HEIGHT;loop++)
    {
        plot_line_original(319,loop,160,120,color);
        if (loop % 20 ==1)
        color ++;
        color %= 16;
    }     

    for (loop =WIDTH-1;loop>-1;loop--)
        {
       plot_line_original(160,120,loop,239,color);
        if (loop % 20 ==1)
        color ++;
        color %= 16;
        }     


    for (loop =HEIGHT-1;loop>-1;loop--)
    {
           plot_line_original(160,120,0,loop  ,color);
        if (loop % 20 ==1)
        color ++;
        color %= 16;
        }             
        color ++;
        color %= 16;

  

    }
#endif

    }

