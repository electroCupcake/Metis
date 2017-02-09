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

#ifndef _BUILD_UTIL_H
#define _BUILD_UTIL_H

#include "types.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <yxml.h>

//setup get prim data
void initBuildUtil();
//reset get prim
int resetBuildUtil();
//set xml data pointer to use for data extraction 
int setXMLdata(char const *p_xmlData);
//finds the attribute and stores the result in stringBuffer, returns 0 if found, -1 if not
int findXMLattr(char const * const p_attr);
//finds the element and stores the result in stringBuffer, returns 0 if found, -1 if not
int findXMLelem(char const * const p_elem);
//finds an element name that starts a block of data, returns 0 if found , -1 if not
int findXMLblock(char const * const p_block);
//gets content if attribute or element is found, puts data in stringBuffer, returns 0 if found, -1 if not
int getXMLcontent();
//get string buffer contents
char *getStringBuf();
//finds short vertex data out of the xml, 0 if found, -1 if not found
int findSVertex(struct s_svertex *p_vertex, char const * const p_vertexName);
//finds long vertex data out of the xml, 0 if found, -1 if not found
int findLVertex(struct s_lvertex *p_vertex, char const * const p_vertexName);
//finds color data out of the xml, 0 if found, -1 if not found
int findColor(struct s_color *color, char const * const p_colorName);
//sets the current block data pointer so we can loop within the same block of data
void setXMLblock();
//reset data back to start, allows data to be in any order
void resetXMLstart();
//reset data back to where setXMLblock was last called, allows a block to be searched in its range
void resetXMLblock();

#endif
