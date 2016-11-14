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

#ifndef _TYPES_H
#define _TYPES_H

#ifndef PROCESS_SUCCESS
  #define PROCESS_SUCCESS 0
#endif

#ifndef PROCESS_FAILURE
  #define PROCESS_FAILURE -1
#endif

#include <stdint.h>

enum en_primType {TYPE_F4, TYPE_FT4, TYPE_G4, TYPE_GT4, TYPE_SPRITE, TYPE_TILE, NONE};

enum en_objectType {PLAYER, ENEMY, FRIEND, STATIC, MOVABLE, NONE};

struct
{
  en_primType type;
  char *p_string
} s_primLookup;

struct s_primLookup primLookup[] = {{TYPE_F4, "TYPE_F4"}, {TYPE_FT4, "TYPE_FT4"}, {TYPE_G4, "TYPE_G4"}, {TYPE_GT4, "TYPE_GT4"}, {TYPE_SPRITE, "TYPE_SPRITE"}, {TYPE_TILE, "TYPE_TILE"}, {NONE, "END"}};

struct
{
  en_objectType type;
  char *p_string;
} s_objectLookup;

struct s_objectLookup objectLookup[] = {{PLAYER, "PLAYER"}, {ENEMY, "ENEMY"}, {FRIEND, "FRIEND"}, {STATIC, "STATIC"}, {MOVABLE, "MOVABLE"}, {NONE, "END"}};

struct s_gamePad
{
  uint8_t status:8;

  uint8_t recvSize:4;
  uint8_t type:4;
  
  uint8_t select:1;
  uint8_t na2:1;
  uint8_t na1:1;
  uint8_t start:1;
  uint8_t up:1;
  uint8_t right:1;
  uint8_t down:1;
  uint8_t left:1;

  uint8_t l2:1;
  uint8_t r2:1;
  uint8_t l1:1;
  uint8_t r1:1;
  uint8_t triangle:1;
  uint8_t circle:1;
  uint8_t ex:1;
  uint8_t square:1;
};

struct s_xmlData
{
  char string[256];
};

struct s_psxBuffer
{
  struct s_primitive *p_primitive;
  unsigned long *p_ot;
  DISPENV disp;
  DRAWENV draw;
};

struct s_svector
{
  int16_t vx;
  int16_t vy;
  int16_t vz;
  int16_t pad;
};

struct s_lvector
{
  int32_t vx;
  int32_t vy;
  int32_t vz;
  int32_t pad;
};

struct s_matrix
{
  int16_t m[3][3];
  int32_t t[3];
};

struct s_dimensions
{
  uint32_t w;
  uint32_t h;
};

struct s_color
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

struct s_tpage
{
  unsigned long *tag;
  unsigned long code[2];
};

struct s_texture
{
  unsigned short id;
  uint32_t size;
  
  char file[256];
  
  struct s_svector vector0;
  struct s_svector vramVector;
  struct s_dimensions dimensions;
  struct s_tpage tpage;
  
  uint8_t *p_data;
};

struct s_translation
{  
  struct s_lvertex transCoor;
  struct s_lvertex scaleCoor;
  
  struct s_svertex rotCoor;
  
  struct s_matrix matrix;
};

struct s_object
{
  enum en_primType primType;
  enum en_objectType objectType;
  
  struct
  {
    struct s_svector vector0;
    struct s_svector vector1;
    struct s_svector vector2;
    struct s_svector vector3;
    
    struct s_color color0;
    struct s_color color1;
    struct s_color color2;
    struct s_color color3;
    
    struct s_texture *p_texture;
  } local;
  
  struct s_translation world;
};

struct s_environment
{
  int numObjects;
  int otSize;
  
  struct s_object **pp_objects;
  
  struct
  {
    struct
    {
      struct s_svector origin;
      struct s_dimensions dimensions;
    } local;
    
    struct s_translation screen;
    
  } world;
  
  struct s_psxBuffer scrBuf[DOUBLE_BUF];
  
  struct s_psxBuffer *p_curScrBuf;
  
  struct 
  {
    char *p_title;
    char *p_message;
    int  *p_data;
  } envMessage;

  //function pointers for callbacks
};

#endif