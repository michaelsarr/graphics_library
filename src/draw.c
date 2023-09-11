#include "draw.h"

/*
need some type of bounds checking
or not?
*/



void setPixel(int16_t x, int16_t y, uint8_t color) 
{
 // if (color > 15)
//printf("%d,%d : %d\n",x,y,color);


    RIA_ADDR0 = (x / 2) + (y * (WIDTH / 2));
    if (x%2)
      RIA_RW0 = (RIA_RW0 & 15) | (color << 4);
    else
      RIA_RW0 = (RIA_RW0 & 240) | color;


}
void plot_line_original(int16_t x0, int16_t y0, int16_t  x1, int16_t y1,uint8_t color)
{

  int16_t dx =  abs (x1 - x0), sx = x0 < x1 ? 1 : -1;
  int16_t dy = -abs (y1 - y0), sy = y0 < y1 ? 1 : -1; 
  int16_t err = dx + dy, e2; /* error value e_xy */
 
  for (;;){  /* loop */
    setPixel(x0,y0,color);
    if (x0 == x1 && y0 == y1) break;
    e2 = 2 * err;
    if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
    if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
  }
}// end drawline

void plot_line(int16_t x0, int16_t y0, int16_t  x1, int16_t y1,uint8_t color)
{
  RIA_ADDR0 = 0xE000;
  RIA_STEP0 =1;
// is the RIA_RW0 8 bits only
RIA_RW0 = *((uint8_t * )(&x0));
RIA_RW0 = *(((uint8_t * )(&x0))+1);
RIA_RW0 = *((uint8_t * )(&y0));
RIA_RW0 = *(((uint8_t * )(&y0))+1);
RIA_RW0 = *((uint8_t * )(&x1));
RIA_RW0 = *(((uint8_t * )(&x1))+1);
RIA_RW0 = *((uint8_t * )(&y1));
RIA_RW0 = *(((uint8_t * )(&y1))+1);
RIA_RW0 = color;

  xreg(0xE000,32,1);

}// end drawline



void plot_circle (int16_t xm, int16_t ym, int16_t r,uint8_t color)
{
  int16_t x = -r, y = 0, err = 2-2*r; /* II. Quadrant */ 
  
  RIA_ADDR0 = 0xE000;
  RIA_STEP0 =1;

RIA_RW0 = *((uint8_t * )(&xm));
RIA_RW0 = *(((uint8_t * )(&xm))+1);
RIA_RW0 = *((uint8_t * )(&ym));
RIA_RW0 = *(((uint8_t * )(&ym))+1);
RIA_RW0 = *((uint8_t * )(&r));
RIA_RW0 = *(((uint8_t * )(&r))+1);

  RIA_RW0 = color;

  xreg(0xE000,33,1);


}




void plot_circle_original (int16_t xm, int16_t ym, int16_t r,uint8_t color)
{
   int16_t x = -r, y = 0, err = 2-2*r; /* II. Quadrant */ 
   do {
      setPixel (xm-x, ym+y,color); /*   I. Quadrant */
      setPixel (xm-y, ym-x,color); /*  II. Quadrant */
      setPixel (xm+x, ym-y,color); /* III. Quadrant */
      setPixel (xm+y, ym+x,color); /*  IV. Quadrant */
      r = err;
      if (r >  x) err += ++x*2+1; /* e_xy+e_x > 0 */
      if (r <= y) err += ++y*2+1; /* e_xy+e_y < 0 */
   } while (x < 0);
}

void raster_circle (int16_t x0, int16_t y0, int16_t radius, uint8_t color)
{
  int16_t f = 1 - radius;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * radius;
  int16_t x = 0;
  int16_t y = radius;
 
  setPixel (x0, y0 + radius,color);
  setPixel (x0, y0 - radius,color);
  setPixel (x0 + radius, y0,color);
  setPixel (x0 - radius, y0,color);
  while (x < y)
  {
    // ddF_x == 2 * x + 1;
    // ddF_y == -2 * y;
    // f == x*x + y*y - radius*radius + 2*x - y + 1;
    if (f >= 0) 
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;    
    setPixel (x0 + x, y0 + y,color);
    setPixel (x0 - x, y0 + y,color);
    setPixel (x0 + x, y0 - y,color);
    setPixel (x0 - x, y0 - y,color);
    setPixel (x0 + y, y0 + x,color);
    setPixel (x0 - y, y0 + x,color);
    setPixel (x0 + y, y0 - x,color);
    setPixel (x0 - y, y0 - x,color);
  }
}

void plot_ellipse_rect (int x0, int y0, int x1, int y1,uint8_t color)
{
   int a = abs (x1 - x0), b = abs (y1 - y0), b1 = b & 1; /* values of diameter */
   long dx = 4 * (1 - a) * b * b, dy = 4 * (b1 + 1) * a * a; /* error increment */
   long err = dx + dy + b1 * a * a, e2; /* error of 1.step */

   if (x0 > x1) { x0 = x1; x1 += a; } /* if called with swapped points */
   if (y0 > y1) y0 = y1; /* .. exchange them */
   y0 += (b + 1) / 2;
   y1 = y0-b1;   /* starting pixel */
   a *= 8 * a; b1 = 8 * b * b;
   do
   {
       setPixel (x1, y0,color); /*   I. Quadrant */
       setPixel (x0, y0,color); /*  II. Quadrant */
       setPixel (x0, y1,color); /* III. Quadrant */
       setPixel (x1, y1,color); /*  IV. Quadrant */
       e2 = 2 * err;
       if (e2 >= dx)
       {
          x0++;
          x1--;
          err += dx += b1;
       } /* x step */
       if (e2 <= dy)
       {
          y0++;
          y1--;
          err += dy += a;
       }  /* y step */ 
   } while (x0 <= x1);
   while (y0-y1 < b)
   {  /* too early stop of flat ellipses a=1 */
       setPixel (x0-1, y0,color); /* -> finish tip of ellipse */
       setPixel (x1+1, y0++,color); 
       setPixel (x0-1, y1,color);
       setPixel (x1+1, y1--,color); 
   }
}

void plot_basic_bezier (int x0, int y0, int x1, int y1, int x2, int y2,uint8_t color)
{                            
  int sx = x0 < x2 ? 1 : -1;
  int sy = y0 < y2 ? 1 : -1; /* step direction */
  int cur = sx * sy *((x0 - x1) * (y2 - y1) - (x2 - x1) * (y0 - y1)); /* curvature */
  int x = x0 - 2 * x1 + x2, y = y0 - 2 * y1 +y2, xy = 2 * x * y * sx * sy;
                                /* compute error increments of P0 */
  long dx = (1 - 2 * abs (x0 - x1)) * y * y + abs (y0 - y1) * xy - 2 * cur * abs (y0 - y2);
  long dy = (1 - 2 * abs (y0 - y1)) * x * x + abs (x0 - x1) * xy + 2 * cur * abs (x0 - x2);
                                /* compute error increments of P2 */
  long ex = (1 - 2 * abs (x2 - x1)) * y * y + abs (y2 - y1) * xy + 2 * cur * abs (y0 - y2);
  long ey = (1 - 2 * abs (y2 - y1)) * x * x + abs (x2 - x1) * xy - 2 * cur * abs (x0 - x2);
                              /* sign of gradient must not change */
  assert ((x0 - x1) * (x2 - x1) <= 0 && (y0 - y1) * (y2 - y1) <= 0); 
  if (cur == 0)
  { /* straight line */
    plot_line (x0, y0, x2, y2,color);
    return;
  }
  x *= 2 * x;
  y *= 2 * y;
  if (cur < 0)
  { /* negated curvature */
    x = -x;
    dx = -dx;
    ex = -ex;
    xy = -xy;
    y = -y;
    dy = -dy;
    ey = -ey;
  }
  /* algorithm fails for almost straight line, check error values */
  if (dx >= -y || dy <= -x || ex <= -y || ey >= -x)
  {        
    plot_line (x0, y0, x1, y1,color); /* simple approximation */
    plot_line (x1, y1, x2, y2,color);
    return;
  }
  dx -= xy;
  ex = dx + dy;
  dy -= xy; /* error of 1.step */
  for (;;)
  { /* plot curve */
    setPixel (x0, y0,color);
    ey = 2 * ex - dy; /* save value for test of y step */
    if (2 * ex >= dx)
    { /* x step */
      if (x0 == x2) break;
      x0 += sx;
      dy -= xy;
      ex += dx += y; 
    }
    if (ey <= 0)
    { /* y step */
      if (y0 == y2) break;
      y0 += sy;
      dx -= xy;
      ex += dy += x; 
    }
  }
}  
  




