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
* See Header for information about this source file.
* 
* Version: v0.0.0
*
*/
#include "engine.h"
#include "graphics.h"

struct
{
  int numObjects;
  
  struct
  {
    struct
    {
      struct s_object **pp_objects;
    } local;
    
    struct s_translation screen;
    
  } world;
  
  struct 
  {
    char *p_title;
    char *p_message;
    int  *p_data;
  } envMessage;
  
} g_environment;

struct 
{
  //function pointers for callbacks
  void (*graphics)(g_environment *);
  void (*controllerHandler)(void);
} g_callbacks;

//setup needed engine requirments
void initEngine(int const width, int const height, int const depth)
{
  memset(&g_environment, 0, sizeof(g_environment));
  
  initGraphics(width, height, depth);
  
  g_callbacks.graphics = &graphicsCallback;
}

//process callback handlers
void processEngine()
{
  g_callbacks.graphics(&g_environment);
}