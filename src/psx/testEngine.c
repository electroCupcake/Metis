/*
 * Metis
 *
 *  Created on: Sep 17, 2016
 *      Author: John Convertino
 *		email: electrobs@gmail.com
 *		
 *		DESCRIPTION
 *
 *		Version: ADD_HERE
 *		Sep 17, 2016 *VERSION*	*NOTES*
 *
 *		TODO
 */

#include "testEngine.h"
//initialize graphics for either psx
void *initGraphics()
{
  //playstation init
  //check memory location to see if the unit is PAL or NTSC
  switch(*(char *)0xbfc7ff52)
  {
    case 'E':
      SetVideoMode(MODE_PAL);
      break;
    default:
      SetVideoMode(MODE_NTSC);
      break;
  }

  //initialize playstation graphics hardware
  GsInitGraph(SCREEN_WIDTH, SCREEN_HEIGHT, GsINTER | GsOFSGPU, 1, 0);
  //setup display buffer
  GsDefDispBuff(0, 0, 0, SCREEN_HEIGHT);

  return NULL;
}

//setup input
void initInput(void *window)
{
  //playstation
  //initialize gamePad buffers which will hold the result of the callback function internal to the library.
  PadInitDirect(g_gamePad[0], g_gamePad[1]);
  //start communication
  PadStartCom();
  //set are own callback to update are control input buffer based on the playstations control buffer
  VSyncCallback(inputCallback);

}

//universal method of moving the primitive on screen (at this point does a warp to the other side if you touch it)
void movePrimitive(struct s_polyF4 *f4, enum inputTypes decodedInput)
{
  static int previousTime = -60;
  switch(decodedInput)
  {
    case UP:
      f4->y0--;
      f4->y1--;
      f4->y2--;
      f4->y3--;
      if(f4->y0 < 0)
      {
	f4->y0 = SCREEN_HEIGHT - SQUARE_SIZE;
	f4->y1 = SCREEN_HEIGHT - SQUARE_SIZE;
	f4->y2 = SCREEN_HEIGHT;
	f4->y3 = SCREEN_HEIGHT;
      }
      break;
    case DOWN:
      f4->y0++;
      f4->y1++;
      f4->y2++;
      f4->y3++;
      if(f4->y2 > SCREEN_HEIGHT)
      {
	f4->y0 = 0;
	f4->y1 = 0;
	f4->y2 = SQUARE_SIZE;
	f4->y3 = SQUARE_SIZE;
      }
      break;
    case LEFT:
      f4->x0--;
      f4->x1--;
      f4->x2--;
      f4->x3--;
      if(f4->x0 < 0)
      {
	f4->x0 = SCREEN_WIDTH - SQUARE_SIZE;
	f4->x1 = SCREEN_WIDTH;
	f4->x2 = SCREEN_WIDTH - SQUARE_SIZE;
	f4->x3 = SCREEN_WIDTH;
      }
      break;
    case RIGHT:
      f4->x0++;
      f4->x1++;
      f4->x2++;
      f4->x3++;
      if(f4->x1 > SCREEN_WIDTH)
      {
	f4->x0 = 0;
	f4->x1 = SQUARE_SIZE;
	f4->x2 = 0;
	f4->x3 = SQUARE_SIZE;
      }
      break;
    case CHANGE_COLOR:
      //if time passed is greater than 60 vsync counts(frame rate of 1/60 means this is 1 second)
      if((VSync(-1) - previousTime) > 60)
      {
	previousTime = VSync(-1);
	f4->r0 = rand()%256;
	f4->g0 = rand()%256;
	f4->b0 = rand()%256;
      }
      break;
    default:
      break;
  }
}

//generate primitive
struct s_polyF4 genPrimitive()
{
  struct s_polyF4 temp;

  SetPolyF4((POLY_F4 *)&temp);

  temp.r0 = 0;
  temp.g0 = 0;
  temp.b0 = 255;

  temp.x0 = 0;
  temp.y0 = 0;
  temp.x1 = SQUARE_SIZE;
  temp.y1 = 0;
  temp.x2 = 0;
  temp.y2 = SQUARE_SIZE;
  temp.x3 = SQUARE_SIZE;
  temp.y3 = SQUARE_SIZE;

  return temp;

}

//display primitive, window is only used for glfw, pass NULL for psx
void display(void *window, struct s_polyF4 *f4)
{
  //playstation
  //clear drawing area
  GsClearDispArea(0, 0, 0);
  //draw primitive to the drawing area (blocks waiting in queue, non-blocking once its up to draw)
  DrawPrim(f4);
  //wait till all drawing is done
  DrawSync(0);
  //wait for vsync (1/60)
  VSync(0);
  //swap display and draw buffer
  GsSwapDispBuff();
}

//does nothing for the psx
void cleanup(void *window)
{

}

//check if the window is still open, since the PSX does not have a window, it returns a 1 to run infinitely
int displayLive(void *window)
{
  return 1;
}

//decode gamePad buffer on vsync callback
static void inputCallback()
{
  switch((g_gamePad[0][2] << 8) | g_gamePad[0][3])
  {
    case 0xFFBF:
      //x press
      g_decodedInput = CHANGE_COLOR;
      break;
    case 0xEFFF:
      //up
      g_decodedInput = UP;
      break;
    case 0xDFFF:
      //right
      g_decodedInput = RIGHT;
      break;
    case 0xBFFF:
      //down
      g_decodedInput = DOWN;
      break;
    case 0x7FFF:
      //left
      g_decodedInput = LEFT;
      break;
    default:
      g_decodedInput = NONE;
      break;
  }
}
