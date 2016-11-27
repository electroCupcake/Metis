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

#include "controller.h"

#define MAX_PLAYERS 2
struct
{
  struct s_gamePad gamePad;
  struct s_object *p_object;
  void (*movement)(struct s_gamePad gamePad, struct s_object *p_object);
} g_controlData[MAX_PLAYERS]

void initController()
{
  PadInitDirect((u_char *)&g_controlData[0].gamePad, (u_char *)&g_controlData[1].gamePad);
  PadStartCom();
}

void registerControllerOne(void (*movement)(struct s_gamePad gamePad, struct s_object *p_object), struct s_object *p_object)
{
  g_controlData[0].movement = movement;
  g_controlData[0].p_object = p_object;
}

void registerControllerTwo(void (*movement)(struct s_gamePad gamePad, struct s_object *p_object), struct s_object *p_object)
{
  g_controlData[1].movement = movement;
  g_controlData[1].p_object = p_object;
}

void processControllers()
{
  if(g_controlData[0].movement != NULL)
  {
    g_controlData[0].movement(g_controlData[0].gamePad, g_controlData[0].p_object);
  }
  
  if(g_controlData[1].movement != NULL)
  {
    g_controlData[1].movement(g_controlData[1].gamePad, g_controlData[1].p_object);
  }
}