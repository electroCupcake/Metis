/*
* Metis 2D Game Engine
* 2016 John Convertino
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*
* The full License is located in the root directory of this project, named LICENSE.txt.
*
* Developed By: John Convertino
*
* INFORMATION ABOUT THE HEADER AND ITS CONTENTS (WHAT IT ALLOWS YOU TO    * DO).
*
* Status: v0.0.0 -NADA
* Previous
*/ 

#include <libgpu.h>
#include "graphics.h"

#define DOUBLE_BUF 2
#define COMPRESS_Z 4

struct s_primitive
{
  void *data;
  enum en_primType type;
};

struct s_buffer
{
  struct s_primitive *p_primitive;
  unsigned long *p_ot;
  DISPENV disp;
  DRAWENV draw;
};

struct s_psx
{
  int width;
  int height;
  int depth;
  
  struct s_buffer buffer[DOUBLE_BUF];
  
  struct s_buffer *p_currBuffer;
  
  struct s_environment *p_environment;
};

struct s_psx g_psx;

//utility functions, all after public functions
//clear vram of all contents
void clearVRAM();
void swapBuffers();

void initGraphics(int const width, int const height, int const depth, struct s_environment *p_environment)
{
  int index;
  int bufIndex;
  
  g_psx.width = width;
  g_psx.height = height;
  g_psx.depth = depth/COMPRESS_Z;
  g_psx.p_primitive = NULL;
  g_psx.p_environment = p_environment;
  
  for(bufIndex = 0; bufIndex < DOUBLE_BUF; bufIndex++)
  {
    g_psx.buffer[bufIndex].p_ot = calloc(g_psx.depth, sizeof(*p_ot));
    g_psx.buffer[bufIndex].p_ot->data = NULL;
  }
  
  clearVRAM();
  
  // within the BIOS, if the address 0xBFC7FF52 equals 'E', set it as PAL (1). Otherwise, set it as NTSC (0)
  switch(*(char *)0xbfc7ff52=='E')
  {
    case 'E':
      SetVideoMode(MODE_PAL); 
      break;
    default:
      SetVideoMode(MODE_NTSC); 
      break;	
  }
  
  ResetCallback();
  ResetGraph(0);

  SetDefDispEnv(&g_psx.buffer[bufIndex].disp, 0, 0, g_psx.width, g_psx.height);
  SetDefDrawEnv(&g_psx.buffer[bufIndex].draw, 0, g_psx.height, g_psx.width, g_psx.height);

  SetDefDispEnv(&g_psx.buffer[bufIndex].disp, 0, g_psx.height, g_psx.width, g_psx.height);
  SetDefDrawEnv(&g_psx.buffer[bufIndex].draw, 0, 0, g_psx.width, g_psx.height);
  
  for(bufIndex = 0; bufIndex < DOUBLE_BUF; bufIndex++)
  {
    g_psx.buffer[bufIndex].draw.isbg = 1;
    g_psx.buffer[bufIndex].draw.r0 = 0;
    g_psx.buffer[bufIndex].draw.g0 = 0;
    g_psx.buffer[bufIndex].draw.b0 = 0;
    
    ClearOTag(g_psx.buffer[bufIndex].p_ot, g_psx.depth);
  }
  
  g_psx.p_currBuffer = g_psx.buffer;
  
  #ifdef DEBUG
    FntLoad(960, 256);
    SetDumpFnt(FntOpen(5, 20, g_psx.width, g_psx.height, 0, 512));
  #endif
    
  InitGeom();
  
  SetGraphDebug(0);
  
  SetGeomScreen(g_psx.depth * COMPRESS_Z);
  
  SetDispMask(1);
}

void transform()
{
  
}

void display()
{
  while(DrawSync(1));
  
  Vsync(0);
  
  FntFlush(-1);
  
  PutDrawEnv(&g_psx.p_currBuffer->draw);
  PutDispEnv(&g_psx.p_currBuffer->disp);
  
  DrawOTag(&g_psx.p_currBuffer->p_ot);
  
  swapBuffers();
  
  #ifdef DEBUG
    FntPrint("%s\n%s\n%X", g_psx.p_environment->message.p_title, g_psx.p_environment->message.p_message, g_psx.p_environment->p_data);
  #endif
}

void clearVRAM()
{
  struct RECT vramArea;
  
  setRECT(&vramArea, 0, 0, 1024, 512);
  
  ClearImage(&vramArea, 0, 0, 0);
  
  while(DrawSync(1));
}

void swapBuffers()
{
  g_psx.p_currBuffer = (g_psx.p_currBuffer == g_psx.buffer ? g_psx.buffer + 1 : g_psx.buffer);
}