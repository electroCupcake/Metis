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

#include "objectBuilder.h"
#include <string.h>
#include <stdio.h>
#include <yxml.h>

#define DEFAULT_BUFSIZE 2048

//holds data relating to xml parsing
struct
{
  int bufSize;
  char stringBuffer[256];
  
  yxml_t yxml;
  
  char p_stack[DEFAULT_BUFSIZE];
  char const *p_xmlData;
  char const *p_xmlDataBlock;
  char const *p_xmlDataStart;
  
} g_parserData;


//defines of names for xmltypes
#define XML_TYPE_NAME "primtype"
#define XML_TYPE_OBJ  "objecttype"
#define XML_VECTOR_0  "vector0"
#define XML_VECTOR_1  "vector1"
#define XML_VECTOR_2  "vector2"
#define XML_VECTOR_3  "vector3"
#define XML_VRAM      "vramVector"
#define XML_X_CORR    "x"
#define XML_Y_CORR    "y"
#define XML_Z_CORR    "z"
#define XML_COLOR_0   "color0"
#define XML_COLOR_1   "color1"
#define XML_COLOR_2   "color2"
#define XML_COLOR_3   "color3"
#define XML_RED	      "red"
#define XML_GREEN     "green"
#define XML_BLUE      "blue"
#define XML_WIDTH     "width"
#define XML_HEIGHT    "height"
#define XML_TEXTURE   "texture"
#define XML_LOCAL     "local"
#define XML_WORLD     "world"
#define XML_FILE      "file"

//helper functions
//finds the attribute and stores the result in stringBuffer, returns 0 if found, -1 if not
int findXMLattr(char const * const p_attr);
//finds the element and stores the result in stringBuffer, returns 0 if found, -1 if not
int findXMLelem(char const * const p_elem);
//finds an element name that starts a block of data, returns 0 if found , -1 if not
int findXMLblock(char const * const p_block);
//gets content if attribute or element is found, puts data in stringBuffer, returns 0 if found, -1 if not
int getXMLcontent();
//finds short vector data out of the xml, 0 if found, -1 if not found
int findSVector(struct s_svector *p_vector, char const * const p_vectorName);
//finds long vector data out of the xml, 0 if found, -1 if not found
int findLVector(struct s_lvector *p_vector, char const * const p_vectorName);
//finds color data out of the xml, 0 if found, -1 if not found
int findColor(struct s_color *color, char const * const p_colorName);
//sets the current block data pointer so we can loop within the same block of data
void setXMLblock();
//reset data back to start, allows data to be in any order
void resetXMLstart();
//reset data back to where setXMLblock was last called, allows a block to be searched in its range
void resetXMLblock();

//setup get prim data
void initObjectBuilder()
{
  g_parserData.p_xmlData = NULL;
  g_parserData.p_xmlDataStart = NULL;
  
  g_parserData.bufSize = DEFAULT_BUFSIZE;
  
  memset(g_parserData.stringBuffer, 0, 256);
  
  yxml_init(&g_parserData.yxml, g_parserData.p_stack, g_parserData.bufSize);
}

//reset get prim
int resetObjectBuilder()
{
  memset(g_parserData.stringBuffer, 0, 256);
  
  memset(&g_parserData.yxml, 0, sizeof(g_parserData.yxml));
  
  memset(g_parserData.p_stack, 0, g_parserData.bufSize);
  
  yxml_init(&g_parserData.yxml, g_parserData.p_stack, g_parserData.bufSize);
  
  resetXMLstart();
}

//free data
void freeObject(struct s_object **pp_object)
{
  if(pp_object != NULL)
  {
    if((*pp_object)->p_texture != NULL)
    {
      if((*p_object)->p_texture->p_data != NULL)
      {
	free((*p_object)->p_texture->p_data);
      }
      
      free((*p_object)->p_texture);
    }
    
    free(*p_object);
  }
}

//set pointer to data (loaded into pointer else where)
void setXMLdata(char const *p_xmlData)
{
  if(p_xmlData != NULL)
  {
    g_parserData.p_xmlData = p_xmlData;
    g_parserData.p_xmlDataStart = p_xmlData;
  }
}

//get prim data (parse xml)
struct s_object *getObject()
{
  int index;
  int returnValue = 0;
  
  struct s_object *p_object;
  
  if(g_parserData.p_xmlData == NULL)
  {
    fprintf(stderr, "\nXML DATA NULL\n");
    return NULL;
  }
  
  p_object = calloc(1, sizeof(*p_object));
  
  if(p_object == NULL)
  {
    fprintf("\nBAD ALLOC\n");
    return NULL;
  }
  
  memset(p_object, 0, sizeof(*p_object));
  
  p_object->local.p_texture = NULL;
  
  if(findXMLattr(XML_TYPE_NAME) < 0)
  {
    fprintf(stderr, "\nDID NOT FIND PRIMITIVE TYPE\n");
    free(p_object);
    return NULL;
  }
  
  for(index = 0; strcmp(primLookup[index].p_string, "END") != 0; index++)
  {
    if(strcmp(primLookup[index].p_string, g_parserData.stringBuffer) == 0)
    {
      p_object->primType = primLookup[index].type;
      break;
    }
  }
  
  
  if(findXMLattr(XML_TYPE_OBJ) < 0)
  {
    fprintf(stderr, "\nDID NOT FIND OBJECT TYPE\n");
    free(p_object);
    return NULL;
  }
  
  for(index = 0; strcmp(objectLookup[index].p_string, "END") != 0; index++)
  {
    if(strcmp(objectLookup[index].p_string, g_parserData.stringBuffer) == 0)
    {
      p_object->objectType = objectLookup[index].type;
      break;
    }
  }
  
  if(findXMLblock(XML_LOCAL) == 0)
  {
    setXMLblock();
    
    returnValue = findSVector(&p_object->local.vector0, XML_VECTOR_0);
    
    if(returnValue < 0)
    {
      fprintf(stderr, "\nCOULD NOT FIND VERTEX 0\n");
      free(p_object);
      return NULL;
    }
    
    findSVector(&p_object->local.vector1, XML_VECTOR_1);

    findSVector(&p_object->local.vector2, XML_VECTOR_2);
    
    findSVector(&p_object->local.vector3, XML_VECTOR_3);
    
    returnValue = findColor(&p_object->local.color0, XML_COLOR_0);
    
    if(returnValue < 0)
    {
      fprintf(stderr, "\nCOULD NOT FIND COLOR 0\n");
      free(p_object);
      return NULL;
    }
    
    findColor(&p_object->local.color1, XML_COLOR_1);
    
    findColor(&p_object->local.color1, XML_COLOR_2);
    
    findColor(&p_object->local.color1, XML_COLOR_3);
    
     //get texture info
    if(findXMLblock(XML_TEXTURE) == 0)
    {
      //keep us within this block now that we have found it.
      setXMLblock();
      
      p_object->local.p_texture = calloc(1, sizeof(*p_object->local.p_texture));

      if(p_object->local.p_texture == NULL)
      {
	free(p_object);
	return NULL;
      }
      
      returnValue = findSVector(&p_object->local.p_texture->vector0, XML_VECTOR_0);
      
      if(returnValue < 0)
      {
	fprintf(stderr, "\nCOULD NOT FIND VECTOR 0\n");
	free(p_object);
	return NULL;
      }
      
      returnValue = findSVector(&p_object->local.p_texture->vramVector, XML_VRAM);
      
      if(returnValue < 0)
      {
	fprintf(stderr, "\nCOULD NOT FIND VRAM LOCATION\n");
	free(p_object);
	return NULL;
      }
      
      if(findXMLelem(XML_FILE) < 0)
      {
	fprintf(strerr, "\nCOULD NOT FIND FILE NAME\n");
	free(p_object);
	return NULL;
      }
      
      strcpy(p_object->local.p_texture->file, g_parserData.stringBuffer);
    }

    resetXMLstart();
  }
  else
  {
    fprintf(stderr, "\nCOULD NOT FIND LOCAL BLOCK\n");
    free(p_object);
    return NULL;
  }
  
  if(findXMLlblock(XML_WORLD) == 0)
  {
    setXMLblock();
    
    returnValue = findLVector(&p_object->p_texture->vector0, XML_VECTOR_0);
    
    if(returnValue < 0)
    {
      fprintf(stderr, "\nCOULD NOT FIND VECTOR 0\n");
      free(p_object);
      return NULL;
    }
    
    resetXMLstart();
  }
  
  return p_object;
}

//find attributes
int findXMLattr(char const * const p_attr)
{
  yxml_ret_t yxmlState;
  
  do
  {
    yxmlState = yxml_parse(&g_parserData.yxml, *g_parserData.p_xmlData);
    
    switch(yxmlState)
    {
      case YXML_ATTRSTART:
	if(strcmp(g_parserData.yxml.attr, p_attr) == 0)
	{
	  return getXMLcontent();
	}
	break;
      default:
	break;
    }
    g_parserData.p_xmlData++;
  }
  while(*g_parserData.p_xmlData);
  
  return PROCESS_FAILURE;
}

//find elements
int findXMLelem(char const * const p_elem)
{
  yxml_ret_t yxmlState;
  
  do
  {
    yxmlState = yxml_parse(&g_parserData.yxml, *g_parserData.p_xmlData);
    
    switch(yxmlState)
    {
      case YXML_ELEMSTART:
	if(strcmp(g_parserData.yxml.elem, p_elem) == 0)
	{
	  return getXMLcontent();
	}
	break;
      default:
	break;
    }
    g_parserData.p_xmlData++;
  }
  while(*g_parserData.p_xmlData);
  
  return PROCESS_FAILURE;
}

//find element blocks
int findXMLblock(char const * const p_block)
{
  yxml_ret_t yxmlState;

  do
  {
    yxmlState = yxml_parse(&g_parserData.yxml, *g_parserData.p_xmlData);
    
    switch(yxmlState)
    {
      case YXML_ELEMSTART:
	if(strcmp(g_parserData.yxml.elem, p_block) == 0)
	{
	  return PROCESS_SUCCESS;
	}
	break;
      default:
	break;
    }
    g_parserData.p_xmlData++;
  }
  while(*g_parserData.p_xmlData);
  
  return PROCESS_FAILURE;
}

//get content when a element match is found (or attribute).
int getXMLcontent()
{
  int len = 0;
  int index = 0;
  yxml_ret_t yxmlState;

  memset(g_parserData.stringBuffer, 0, 256);
  
  do
  {
    yxmlState = yxml_parse(&g_parserData.yxml, *g_parserData.p_xmlData);
    
    switch(yxmlState)
    {
      case YXML_ATTRVAL:
      case YXML_CONTENT:
	switch(g_parserData.yxml.data[0])
	{
	  case '\n':
	  case '>':
	  case '<':
	    break;
	  default:
	    g_parserData.stringBuffer[index] = g_parserData.yxml.data[0];
	    
	    index++;
	    
	    if(index >= 256)
	    {
	      return PROCESS_FAILURE;
	    }
	    break;
	}
	break;
      case YXML_ATTREND:
      case YXML_ELEMEND:
	return PROCESS_SUCCESS;
	break;
      default:
	break;
    }
    
    g_parserData.p_xmlData++;
  }
  while(*g_parserData.p_xmlData);
    
  return PROCESS_FAILURE;
}

//find short vector data, helps since this needs to happen 4 or more times
int findSVector(struct s_svector *p_vector, char const * const p_vectorName)
{
  if(findXMLblock(p_vectorName) < 0)
  {
    resetXMLstart();
    return PROCESS_FAILURE;
  }
 
  setXMLblock();
 
  if(findXMLelem(XML_X_CORR) < 0)
  {
    resetXMLstart();
    return PROCESS_FAILURE;
  }
  
  p_vector->vx = atoi(g_parserData.stringBuffer);
  
  resetXMLblock();
  
  if(findXMLelem(XML_Y_CORR) < 0)
  {
    resetXMLstart();
    return PROCESS_FAILURE;
  }
  
  p_vector->vy = atoi(g_parserData.stringBuffer);
  
  resetXMLblock();
  
  if(findXMLelem(XML_Z_CORR) < 0)
  {
    p_vector->vz = 0; 
  }
  
  resetXMLblock();
  
  return PROCESS_SUCCESS;
}

//find long vector data, helps since this needs to happen 4 or more times
int findLVector(struct s_lvector *p_vector, char const * const p_vectorName)
{
  if(findXMLblock(p_vectorName) < 0)
  {
    resetXMLstart();
    return PROCESS_FAILURE;
  }
 
  setXMLblock();
 
  if(findXMLelem(XML_X_CORR) < 0)
  {
    resetXMLstart();
    return PROCESS_FAILURE;
  }
  
  p_vector->vx = atoi(g_parserData.stringBuffer);
  
  resetXMLblock();
  
  if(findXMLelem(XML_Y_CORR) < 0)
  {
    resetXMLstart();
    return PROCESS_FAILURE;
  }
  
  p_vector->vy = atoi(g_parserData.stringBuffer);
  
  resetXMLblock();
  
  if(findXMLelem(XML_Z_CORR) < 0)
  {
    p_vector->vz = 0; 
  }
  
  resetXMLblock();
  
  return PROCESS_SUCCESS;
}

//find color data, helps since this happens 4 or more times
int findColor(struct s_color *color, char const * const p_colorName)
{
  if(findXMLblock(p_colorName) < 0)
  {
    resetXMLstart();
    return PROCESS_FAILURE;
  }
 
  setXMLblock();
 
  if(findXMLelem(XML_RED) < 0)
  {
    resetXMLstart();
    return PROCESS_FAILURE;
  }
  
  color->r = atoi(g_parserData.stringBuffer);
  
  resetXMLblock();
  
  if(findXMLelem(XML_GREEN) < 0)
  {
    resetXMLstart();
    return PROCESS_FAILURE;
  }
  
  color->g = atoi(g_parserData.stringBuffer);
  
  resetXMLblock();
  
  if(findXMLelem(XML_BLUE) < 0)
  {
    resetXMLstart();
    return PROCESS_FAILURE;
  }
  
  color->b = atoi(g_parserData.stringBuffer);
  
  resetXMLblock();
  
  return PROCESS_SUCCESS;
}

//set xml block pointer to current spot
void setXMLblock()
{
  g_parserData.p_xmlDataBlock = g_parserData.p_xmlData;
}

//reset current pointer to start of xml data
void resetXMLstart()
{ 
  setXMLdata(g_parserData.p_xmlDataStart);
}

//reset current pointer back to last setXMLblock call
void resetXMLblock()
{
  g_parserData.p_xmlData = g_parserData.p_xmlDataBlock;
}
