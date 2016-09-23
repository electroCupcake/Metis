/*
* Metis 2D Game Engine
* 2016 John Convertino, Jeff Eckert
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
* Developed By: John Convertino, Jeff Eckert
*
* INFORMATION ABOUT THE HEADER AND ITS CONTENTS (WHAT IT ALLOWS YOU TO    * DO).
*
* Status: v0.0.0 -NADA
* Previous
*/

#include "gameObject.h"

enum inputStates {UP = 2, DOWN = 4, LEFT = 8, RIGHT = 16};

enum playerType {AI, PLAYER};

struct s_controller {
  int returnValue;
  void *p_pointerPoint;
}

struct s_controller *createController(int playerNumber);

int attachController(struct s_gameObject gameObject);

int setupController(struct s_gameObject gameObject, enum playerType type, void *map);