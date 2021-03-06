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
#include "controller.h"
#include "gameObject.h"
#include "graphics.h"

struct s_environment g_environment;

//setup needed engine requirments
void initEngine(int const width, int const height, int const depth)
{
  memset(&g_environment, 0, sizeof(g_environment));
  
  //allocate in future to world size of objects, for now, just allocate to 128 to be greater than the intended example
  g_environment.world.local.pp_objects = calloc(128, sizeof(s_object));
  
  g_environment.numObjects = 128;
  
  initGraphics(width, height, depth, &g_environment);
  
  initController();
  
  initGameObject();
}

//process engine
void processEngine()
{
  processControllers();
  
  transform();
  
  display();
}