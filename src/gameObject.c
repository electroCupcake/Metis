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

#include "gameObject.h"
#include "objectBuilder.h"
#include "fileIO.h"

//add utility function to add a object to the list of object
//consider a new data type like a stack
void pushObject(struct s_object *p_object);

struct s_environment *gp_environment = NULL;

void initGameObject(struct s_environment *p_environment)
{
  if(p_environment == NULL)
  {
    #ifdef DEBUG
      prinf("\nENVIRONMENT NULL\n");
    #endif
    return;
  }
  
  initObjectBuilder();
  
  gp_environment = p_environment;
}

struct s_gameObject *createGameObject(char *p_fname)
{
  uint32_t len = 0;
  
  char *p_data = NULL;
  struct s_gameObject *p_gameObject = NULL;
  
  if(p_fname == NULL)
  {
    #ifdef DEBUG
      printf("\nFILE NAME NULL\n");
    #endif
    return NULL;
  }
  
  loadObjectData(p_fname, &p_data, &len);
  
  if(p_data == NULL)
  {
    #ifdef DEBUG
      printf("\nDATA FAILURE\n");
    #endif
    return NULL;
  }
  
  p_gameObject = malloc(sizeof(*p_gameObject));
  
  if(p_gameObject == NULL)
  {
    #ifdef DEBUG
      printf("\nMALLOC FAILURE\n");
    #endif
    return NULL;
  }
  
  p_gameObject->p_data = NULL;
  
  strcpy(p_gameObject->file, p_fname);
  
  resetObjectBuilder();
  
  setXMLdata(p_data);
  
  p_gameObject->p_data = getObject();
  
  if(p_gameObject->p_data == NULL)
  {
    #ifdef DEBUG
      printf("\nGET OBJECT DATA FAIL\n");
    #endif
    return NULL;
  }
  
  //add object to environment here, we give this back to the caller so it can be attached if needed, or safely ignored.
  
  return p_gameObject;
}

//texture loading
void loadGameObjectTexture(struct s_gameObject *p_gameObject)
{
  
}

void destroyGameObject(struct s_gameObject **pp_gameObject)
{
  if(pp_gameObject != NULL)
  {
    if((*pp_gameObject)->p_data != NULL)
    {
      freeObject(&(*pp_gameObject)->p_data);
    }
    
    free(*pp_gameObject);
  }
}