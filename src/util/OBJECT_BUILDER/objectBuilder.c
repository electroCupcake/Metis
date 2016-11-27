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
#include <buildUtil.h>


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

static int sg_id = 0;

struct s_objectLookup objectLookup[] = {{PLAYER, "PLAYER"}, {ENEMY, "ENEMY"}, {FRIEND, "FRIEND"}, {STATIC, "STATIC"}, {MOVABLE, "MOVABLE"}, {NO_OBJ, "END"}};
struct s_primLookup primLookup[] = {{TYPE_F4, "TYPE_F4"}, {TYPE_FT4, "TYPE_FT4"}, {TYPE_G4, "TYPE_G4"}, {TYPE_GT4, "TYPE_GT4"}, {TYPE_SPRITE, "TYPE_SPRITE"}, {TYPE_TILE, "TYPE_TILE"}, {NO_PRIM, "END"}};

//use this to setup xml, but if your using the world builder only call one or the other init
void initObjectBuilder()
{
  initBuildUtil();
}

//get prim data (parse xml)
struct s_object *getObject(char const *p_objectData)
{
  int index;
  int returnValue = 0;
  
  struct s_object *p_object;
  
  resetBuildUtil();
  setXMLdata(p_objectData);
  
  if(g_parserData.p_xmlData == NULL)
  {
    #ifdef DEBUG
      printf("\nXML DATA NULL\n");
    #endif
    return NULL;
  }
  
  p_object = calloc(1, sizeof(*p_object));
  
  if(p_object == NULL)
  {
    #ifdef DEBUG
      printf("\nBAD ALLOC\n");
    #endif
    return NULL;
  }
  
  memset(p_object, 0, sizeof(*p_object));
  
  p_object->local.p_texture = NULL;
  
  if(findXMLattr(XML_TYPE_NAME) < 0)
  {
    #ifdef DEBUG
      printf("\nDID NOT FIND PRIMITIVE TYPE\n");
    #endif
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
    #ifdef DEBUG
      printf("\nDID NOT FIND OBJECT TYPE\n");
    #endif
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
      #ifdef DEBUG
	printf("\nCOULD NOT FIND VERTEX 0\n");
      #endif
      free(p_object);
      return NULL;
    }
    
    findSVector(&p_object->local.vector1, XML_VECTOR_1);

    findSVector(&p_object->local.vector2, XML_VECTOR_2);
    
    findSVector(&p_object->local.vector3, XML_VECTOR_3);
    
    returnValue = findColor(&p_object->local.color0, XML_COLOR_0);
    
    if(returnValue < 0)
    {
      #ifdef DEBUG
	printf("\nCOULD NOT FIND COLOR 0\n");
      #endif
      free(p_object);
      return NULL;
    }
    
    findColor(&p_object->local.color1, XML_COLOR_1);
    
    findColor(&p_object->local.color2, XML_COLOR_2);
    
    findColor(&p_object->local.color3, XML_COLOR_3);
    
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
	#ifdef DEBUG
	  printf("\nCOULD NOT FIND VECTOR 0\n");
	#endif
	free(p_object);
	return NULL;
      }
      
      returnValue = findSVector(&p_object->local.p_texture->vramVector, XML_VRAM);
      
      if(returnValue < 0)
      {
	#ifdef DEBUG
	  printf("\nCOULD NOT FIND VRAM LOCATION\n");
	#endif
	free(p_object);
	return NULL;
      }
      
      if(findXMLelem(XML_FILE) < 0)
      {
	#ifdef DEBUG
	  printf("\nCOULD NOT FIND FILE NAME\n");
	#endif
	free(p_object);
	return NULL;
      }
      
      strcpy(p_object->local.p_texture->file, g_parserData.stringBuffer);
    }

    resetXMLstart();
  }
  else
  {
    #ifdef DEBUG
      printf("\nCOULD NOT FIND LOCAL BLOCK\n");
    #endif
    free(p_object);
    return NULL;
  }
  
  if(findXMLblock(XML_WORLD) == 0)
  {
    setXMLblock();
    
    returnValue = findLVector(&p_object->world.transCoor, XML_VECTOR_0);
    
    if(returnValue < 0)
    {
      #ifdef DEBUG
	printf("\nCOULD NOT FIND VECTOR 0\n");
      #endif
      free(p_object);
      return NULL;
    }
    
    resetXMLstart();
  }
  
  p_object->world.scaleCoor.vx = ONE;
  p_object->world.scaleCoor.vy = ONE;
  p_object->world.scaleCoor.vz = ONE;
  
  p_object->id = sg_id++;
  
  return p_object;
}

//free data
void freeObject(struct s_object **pp_object)
{
  if(pp_object != NULL)
  {
    if((*pp_object)->local.p_texture != NULL)
    {
      if((*pp_object)->local.p_texture->p_data != NULL)
      {
	free((*pp_object)->local.p_texture->p_data);
      }
      
      free((*pp_object)->local.p_texture);
    }
    
    free(*pp_object);
  }
}
