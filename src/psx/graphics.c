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
  int id;
  enum en_primType type;
  void *data;
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
//updateOT with primitives that need to be drawn
void updateOT(struct s_object *p_object, struct s_svector vector0, struct s_svector vector1, struct s_svector vector2, struct s_svector vector3, long zCoor);
//clear vram of all contents
void clearVRAM();
//swap buffers
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
    
  clearVRAM();
  
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

//populateOT via add prim
void populateOT(struct s_object *p_object)
{
  int buffIndex;
  
  short width = p_object->local.vector0.vx - p_object->local.vector1.vx;
  short height = p_object->local.vector0.vy - p_object->local.vector2.vy;
  
  g_psx.p_environment->world.local.pp_object[p_object->id] = p_object;
 
  for(buffIndex = 0; buffIndex < DOUBLE_BUF; buffIndex++)
  {
    g_psx.buffer[buffIndex].p_primitive[p_object->id].type = p_object->primType;
    
    switch(p_object->primType)
    {
      case TYPE_SPRITE:
	g_psx.buffer[buffIndex].p_primitive[p_object->id].data = malloc(sizeof(SPRT));
	SetSprt((SPRT *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data);
	setXY0((SPRT *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data, p_object->local.vector0.vx, p_object->local.vector0.vy);
	setWH((SPRT *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data, width, height);
	setUV0((SPRT *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data, p_object->local.p_texture->vector0.vx, p_object->local.p_texture->vector0.vy);
	setRGB0((SPRT *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data, p_object->local.color0.r, p_object->local.color0.g, p_object->local.color0.b);
	break;
      case TYPE_TILE:
	g_psx.buffer[buffIndex].p_primitive[p_object->id].data = malloc(sizeof(TILE));
	setTile((TILE *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data);
	setXY0((TILE *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data, p_object->local.vector0.vx, p_object->local.vector0.vy);
	setWH((TILE *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data, width, height);
	setRGB0((TILE *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data, p_object->local.color0.r, p_object->local.color0.g, p_object->local.color0.b);
	break;
      case TYPE_F4:
	g_psx.buffer[buffIndex].p_primitive[p_object->id].data = malloc(sizeof(POLY_F4));
	SetPolyF4((POLY_F4 *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data);
	setXYWH((POLY_F4 *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data,  p_object->local.vector0.vx, p_object->local.vector0.vy, width, height);
	setRGB0((POLY_F4 *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data, p_object->local.color0.r, p_object->local.color0.g, p_object->local.color0.b);
	break;
      case TYPE_FT4:
	g_psx.buffer[buffIndex].p_primitive[p_object->id].data = malloc(sizeof(POLY_FT4));
	SetPolyFT4((POLY_FT4 *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data);
	setXYWH((POLY_FT4 *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data,  p_object->local.vector0.vx, p_object->local.vector0.vy, width, height);
	setUVWH((POLY_FT4 *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data, p_object->local.p_texture->vector0.vx, p_object->local.p_texture->vector0.vy, p_object->local.p_texture->dimensions.w, p_object->local.p_texture->dimensions.h);
	setRGB0((POLY_FT4 *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data, p_object->local.color0.r, p_object->local.color0.g, p_object->local.color0.b);
	break;
      case TYPE_G4:
	g_psx.buffer[buffIndex].p_primitive[p_object->id].data = malloc(sizeof(POLY_G4));
	SetPolyG4((POLY_G4 *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data);
	setXYWH((POLY_G4 *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data,  p_object->local.vector0.vx, p_object->local.vector0.vy, width, height);
	setRGB0((POLY_G4 *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data, p_object->local.color0.r, p_object->local.color0.g, p_object->local.color0.b);
	setRGB1((POLY_G4 *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data, p_object->local.color1.r, p_object->local.color1.g, p_object->local.color1.b);
	setRGB2((POLY_G4 *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data, p_object->local.color2.r, p_object->local.color2.g, p_object->local.color2.b);
	setRGB3((POLY_G4 *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data, p_object->local.color3.r, p_object->local.color3.g, p_object->local.color3.b);
	break;
      case TYPE_GT4:
	g_psx.buffer[buffIndex].p_primitive[p_object->id].data = malloc(sizeof(POLY_GT4));
	SetPolyGT4((POLY_GT4 *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data);
	setXYWH((POLY_GT4 *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data,  p_object->local.vector0.vx, p_object->local.vector0.vy, width, height);
	setUVWH((POLY_GT4 *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data, p_object->local.p_texture->vector0.vx, p_object->local.p_texture->vector0.vy, p_object->local.p_texture->dimensions.w, p_object->local.p_texture->dimensions.h);
	setRGB0((POLY_GT4 *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data, p_object->local.color0.r, p_object->local.color0.g, p_object->local.color0.b);
	setRGB1((POLY_GT4 *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data, p_object->local.color1.r, p_object->local.color1.g, p_object->local.color1.b);
	setRGB2((POLY_GT4 *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data, p_object->local.color2.r, p_object->local.color2.g, p_object->local.color2.b);
	setRGB3((POLY_GT4 *)g_psx.buffer[buffIndex].p_primitive[p_object->id].data, p_object->local.color3.r, p_object->local.color3.g, p_object->local.color3.b);
	break;
      default:
	printf("\nERROR, NO TYPE DEFINED AT INDEX %d\n", p_object->id);
	break;
    }
    
    AddPrim(&(g_psx.buffer[buffIndex].p_ot[p_object->id]), g_psx.buffer[buffIndex].p_primitive[p_object->id].data);
  }
}

void transform()
{
  int index;
  long zCoor;
  
  struct s_svector vector0;
  struct s_svector vector1;
  struct s_svector vector2;
  struct s_svector vector3;
  
  ClearOTag(g_psx.p_currBuffer.p_ot, g_psx.depth);
  
  for(index = 0; index < g_psx.p_environment->numObjects; index++)
  {
  //calculation from local to world to screen for each object done here
  //then set it to the current matrix
  //then update the ordering table with the ones needed to be drawn
    updateOT(g_psx.p_environment->world.local.pp_objects[index], vector0, vector1, vector2, vector3);
  }
}

void display()
{
  while(DrawSync(1));
  
  Vsync(0);
  
  #ifdef DEBUG
    FntFlush(-1);
  #endif
  
  PutDrawEnv(&g_psx.p_currBuffer->draw);
  PutDispEnv(&g_psx.p_currBuffer->disp);
  
  DrawOTag(&g_psx.p_currBuffer->p_ot);
  
  swapBuffers();
  
  #ifdef DEBUG
    FntPrint("%s\n%s\n%X", g_psx.p_environment->message.p_title, g_psx.p_environment->message.p_message, g_psx.p_environment->p_data);
  #endif
}

//utility functions
void updateOT(struct s_object *p_object, struct s_svector vector0, struct s_svector vector1, struct s_svector vector2, struct s_svector vector3, long zCoor)
{
  short width = vector0.vx - vector1.vx;
  short height = vector0.vy - vector2.vy;
  
  switch(p_object->type)
  {
    case TYPE_SPRITE:
      setXY0((SPRT *)g_psx.currBuffer.p_primitive[p_object->id].data, vector0.vx, vector0.vy);
      setWH((SPRT *)g_psx.currBuffer.p_primitive[p_object->id].data, width, height);
      setUV0((SPRT *)g_psx.currBuffer.p_primitive[p_object->id].data, p_object->local.p_texture->vector0.vx, p_object->local.p_texture->vector0.vy);
      setRGB0((SPRT *)g_psx.currBuffer.p_primitive[p_object->id].data, p_object->local.color0.r, p_object->local.color0.g, p_object->local.color0.b);
      break;
    case TYPE_TILE:
      setXY0((TILE *)g_psx.currBuffer.p_primitive[p_object->id].data, vector0.vx, vector0.vy);
      setWH((TILE *)g_psx.currBuffer.p_primitive[p_object->id].data, width, height);
      setRGB0((TILE *)g_psx.currBuffer.p_primitive[p_object->id].data, p_object->local.color0.r, p_object->local.color0.g, p_object->local.color0.b);
      break;
    case TYPE_F4:
      setXY0((TILE *)g_psx.currBuffer.p_primitive[p_object->id].data, vector0.vx, vector0.vy);
      setXY1((TILE *)g_psx.currBuffer.p_primitive[p_object->id].data, vector1.vx, vector1.vy);
      setXY2((TILE *)g_psx.currBuffer.p_primitive[p_object->id].data, vector2.vx, vector2.vy);
      setXY3((TILE *)g_psx.currBuffer.p_primitive[p_object->id].data, vector3.vx, vector3.vy);
      setRGB0((POLY_F4 *)g_psx.currBuffer.p_primitive[p_object->id].data, p_object->local.color0.r, p_object->local.color0.g, p_object->local.color0.b);
      break;
    case TYPE_FT4:
      setXY0((TILE *)g_psx.currBuffer.p_primitive[p_object->id].data, vector0.vx, vector0.vy);
      setXY1((TILE *)g_psx.currBuffer.p_primitive[p_object->id].data, vector1.vx, vector1.vy);
      setXY2((TILE *)g_psx.currBuffer.p_primitive[p_object->id].data, vector2.vx, vector2.vy);
      setXY3((TILE *)g_psx.currBuffer.p_primitive[p_object->id].data, vector3.vx, vector3.vy);
      setUVWH((POLY_FT4 *)g_psx.currBuffer.p_primitive[p_object->id].data, p_object->local.p_texture->vector0.vx, p_object->local.p_texture->vector0.vy, p_object->local.p_texture->dimensions.w, p_object->local.p_texture->dimensions.h);
      setRGB0((POLY_FT4 *)g_psx.currBuffer.p_primitive[p_object->id].data, p_object->local.color0.r, p_object->local.color0.g, p_object->local.color0.b);
      break;
    case TYPE_G4:
      setXY0((TILE *)g_psx.currBuffer.p_primitive[p_object->id].data, vector0.vx, vector0.vy);
      setXY1((TILE *)g_psx.currBuffer.p_primitive[p_object->id].data, vector1.vx, vector1.vy);
      setXY2((TILE *)g_psx.currBuffer.p_primitive[p_object->id].data, vector2.vx, vector2.vy);
      setXY3((TILE *)g_psx.currBuffer.p_primitive[p_object->id].data, vector3.vx, vector3.vy);
      setRGB0((POLY_G4 *)g_psx.currBuffer.p_primitive[p_object->id].data, p_object->local.color0.r, p_object->local.color0.g, p_object->local.color0.b);
      setRGB1((POLY_G4 *)g_psx.currBuffer.p_primitive[p_object->id].data, p_object->local.color1.r, p_object->local.color1.g, p_object->local.color1.b);
      setRGB2((POLY_G4 *)g_psx.currBuffer.p_primitive[p_object->id].data, p_object->local.color2.r, p_object->local.color2.g, p_object->local.color2.b);
      setRGB3((POLY_G4 *)g_psx.currBuffer.p_primitive[p_object->id].data, p_object->local.color3.r, p_object->local.color3.g, p_object->local.color3.b);
      break;
    case TYPE_GT4:
      setXY0((TILE *)g_psx.currBuffer.p_primitive[p_object->id].data, vector0.vx, vector0.vy);
      setXY1((TILE *)g_psx.currBuffer.p_primitive[p_object->id].data, vector1.vx, vector1.vy);
      setXY2((TILE *)g_psx.currBuffer.p_primitive[p_object->id].data, vector2.vx, vector2.vy);
      setXY3((TILE *)g_psx.currBuffer.p_primitive[p_object->id].data, vector3.vx, vector3.vy);
      setUVWH((POLY_GT4 *)g_psx.currBuffer.p_primitive[p_object->id].data, p_object->local.p_texture->vector0.vx, p_object->local.p_texture->vector0.vy, p_object->local.p_texture->dimensions.w, p_object->local.p_texture->dimensions.h);
      setRGB0((POLY_GT4 *)g_psx.currBuffer.p_primitive[p_object->id].data, p_object->local.color0.r, p_object->local.color0.g, p_object->local.color0.b);
      setRGB1((POLY_GT4 *)g_psx.currBuffer.p_primitive[p_object->id].data, p_object->local.color1.r, p_object->local.color1.g, p_object->local.color1.b);
      setRGB2((POLY_GT4 *)g_psx.currBuffer.p_primitive[p_object->id].data, p_object->local.color2.r, p_object->local.color2.g, p_object->local.color2.b);
      setRGB3((POLY_GT4 *)g_psx.currBuffer.p_primitive[p_object->id].data, p_object->local.color3.r, p_object->local.color3.g, p_object->local.color3.b);
      break;
    default:
      printf("\nUnknown Type for update at index %d %d\n", index, g_psx.currBuffer.p_primitive[p_object->id].type);
      break;
  }
  
  AddPrim(&(g_psx.currBuffer.p_ot[p_object->id]), g_psx.currBuffer.p_primitive[p_object->id].data);
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