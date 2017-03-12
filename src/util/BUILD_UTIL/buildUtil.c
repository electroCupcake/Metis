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

#include "buildUtil.h"


#define DEFAULT_BUFSIZE 2048

#define XML_RED	      "red"
#define XML_GREEN     "green"
#define XML_BLUE      "blue"
#define XML_X_CORR    "x"
#define XML_Y_CORR    "y"
#define XML_Z_CORR    "z"

//holds data relating to xml parsing
struct
{
  char initRun;
  int bufSize;
  char stringBuffer[256];
  
  yxml_t yxml;
  
  char p_stack[DEFAULT_BUFSIZE];
  char const *p_xmlData;
  char const *p_xmlDataBlock;
  char const *p_xmlDataStart;
  
} g_parserData;

g_parserData.initRun = 0;
//setup get prim data
void initBuildUtil()
{
  if(g_parserData.initRun == 1) return;
  
  g_parserData.p_xmlData = NULL;
  g_parserData.p_xmlDataStart = NULL;
  
  g_parserData.bufSize = DEFAULT_BUFSIZE;
  
  memset(g_parserData.stringBuffer, 0, 256);
  
  yxml_init(&g_parserData.yxml, g_parserData.p_stack, g_parserData.bufSize);
  
  g_parserData.initRun = 1;
}

//reset get prim
int resetBuildUtil()
{
  memset(g_parserData.stringBuffer, 0, 256);
  
  memset(&g_parserData.yxml, 0, sizeof(g_parserData.yxml));
  
  memset(g_parserData.p_stack, 0, g_parserData.bufSize);
  
  yxml_init(&g_parserData.yxml, g_parserData.p_stack, g_parserData.bufSize);
  
  resetXMLstart();
}

//set pointer to data (loaded into pointer else where)
int setXMLdata(char const *p_xmlData)
{
  if(p_xmlData == NULL)
  {
    return PROCESS_FAILURE;
  }
  
  g_parserData.p_xmlData = p_xmlData;
  g_parserData.p_xmlDataStart = p_xmlData;
  
  return PROCESS_SUCCESS;
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

char *getStringBuf()
{
  return g_parserData.stringBuffer;
}

//find short vertex data, helps since this needs to happen 4 or more times
int findSVertex(struct s_svertex *p_vertex, char const * const p_vertexName)
{
  if(findXMLblock(p_vertexName) < 0)
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
  
  p_vertex->vx = atoi(g_parserData.stringBuffer);
  
  resetXMLblock();
  
  if(findXMLelem(XML_Y_CORR) < 0)
  {
    resetXMLstart();
    return PROCESS_FAILURE;
  }
  
  p_vertex->vy = atoi(g_parserData.stringBuffer);
  
  resetXMLblock();
  
  if(findXMLelem(XML_Z_CORR) < 0)
  {
    p_vertex->vz = 0; 
  }
  else
  {
    p_vertex->vz = atoi(g_parserData.stringBuffer);
  }
  
  resetXMLblock();
  
  return PROCESS_SUCCESS;
}

//find long vertex data, helps since this needs to happen 4 or more times
int findLVertex(struct s_lvertex *p_vertex, char const * const p_vertexName)
{
  if(findXMLblock(p_vertexName) < 0)
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
  
  p_vertex->vx = atoi(g_parserData.stringBuffer);
  
  resetXMLblock();
  
  if(findXMLelem(XML_Y_CORR) < 0)
  {
    resetXMLstart();
    return PROCESS_FAILURE;
  }
  
  p_vertex->vy = atoi(g_parserData.stringBuffer);
  
  resetXMLblock();
  
  if(findXMLelem(XML_Z_CORR) < 0)
  {
    resetXMLstart();
    return PROCESS_FAILURE;
  }

  p_vertex->vz = atoi(g_parserData.stringBuffer);

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
