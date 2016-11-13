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

//standard c includes here

#ifndef _ENGINE_H
#define _ENGINE_H

#include "ableIO.h"
#include "ai.h"
#include "collsionDetect.h"
#include "controller.h"
#include "gameObject.h"
#include "graphics.h"
#include "physics.h"
#include "physicsProcessor.h"

struct s_engine {
  int returnValue;
  void *p_pointerPoint;
}

struct s_engine *initEngine();

void freeEngine(struct s_engine **engine);

#endif