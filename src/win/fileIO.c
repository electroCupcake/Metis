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

#include "fileIO.h"

//utility functions
void readFile(char *p_fname, char **op_data, uint32_t *op_len);
void writeFile(char *p_fname, char *ip_data, uint32_t len);

//load a data from CD, returns address pointer in memory.
void loadObjectData(char *p_fname, char **op_data, uint32_t *op_len)
{
  readFile(p_fname, op_data, op_len);
}
//read from the memory card and return pointer to data
void loadSaveData(char *p_fname, char **op_data, uint32_t len)
{
  readFile(p_fname, op_data, &len);
}
//write to the memory card using data passed to it.
void saveSaveData(char *p_fname, char *ip_data, uint32_t len)
{
  writeFile(p_fname, ip_data, len);
}

void readFile(char *p_fname, char **op_data, uint32_t *op_len)
{
  int size = 0;
  int returnValue = 0;
  
  FILE *p_file = NULL;
  
  if(op_len == NULL)
  {
    #ifdef DEBUG
      printf("\nLEN POINTER NULL\n");
    #endif
    return;
  }
  
  if(op_data == NULL)
  {
    #ifdef DEBUG
      printf("\nOUTPUT POINTER NULL\n");
    #endif
    return;
  }
  
  if(*op_data != NULL)
  {
    #ifdef DEBUG
      printf("\nOUTPUT DATA NOT NULL\n");
    #endif
    return;
  }
  
  if(p_fname == NULL)
  {
    #ifdef DEBUG
      printf("\nFILE NAME NULL\n");
    #endif
    return;
  }
  
  p_file = fopen(p_fname, "rb");
  
  if(p_file == NULL)
  {
    #ifdef DEBUG
      printf("\nFILE POINTER NULL\n");
    #endif
    return;
  }
  
  returnValue = fseek(p_file, 0, SEEK_END);
  
  if(returnValue != 0)
  {
    #ifdef DEBUG
      printf("\nFSEEK FAIL\n");
    #endif
    return;
  }
  
  size = ftell(p_file) + 1;
  
  if(size <= 0)
  {
    #ifdef DEBUG
      printf("\nFTELL FAIL\n");
    #endif
    return;
  }
    
  returnValue = fseek(p_file, 0, SEEK_SET);
  
  if(returnValue != 0)
  {
    #ifdef DEBUG
      printf("\nFSEEK FAIL\n");
    #endif
    return;
  }
  
  *op_data = malloc(size);
  
  if(*op_data == NULL)
  {
    #ifdef DEBUG
      printf("\nMALLOC FAIL\n");
    #endif
    return;
  }
  
  do
  {
    int numRead = 0;
    
    numRead = fread(*op_data, sizeof(char), size, p_file);
    
    if(numRead != size)
    {
      #ifdef DEBUG
	printf("\nREAD FAIL\n");
      #endif
      return;
    }
  }
  while(feof(p_file) == 0)
    
  *op_len = size;
    
  fclose(p_file);
  
  #ifdef DEBUG
    printf("\nREAD COMPLETE\n");
  #endif
}

void writeFile(char *p_fname, char *ip_data, uint32_t len)
{
  int returnValue = 0;
  
  FILE *p_file = NULL;
  
  if(ip_data == NULL)
  {
    #ifdef DEBUG
      printf("\nINPUT POINTER NULL\n");
    #endif
    return;
  }
  
  if(p_fname == NULL)
  {
    #ifdef DEBUG
      printf("\nFILE NAME NULL\n");
    #endif
    return;
  }
  
  p_file = fopen(p_fname, "wb");
  
  if(p_file == NULL)
  {
    #ifdef DEBUG
      printf("\nFILE POINTER NULL\n");
    #endif
    return;
  }
  
  int numWrote = 0;
  
  numWrote = fread(ip_data, sizeof(char), size, p_file);
  
  if(numWrote != size)
  {
    #ifdef DEBUG
      printf("\nWRITE FAIL\n");
    #endif
    return;
  }
    
  fclose(p_file);
  
  #ifdef DEBUG
    printf("\nWRITE COMPLETE\n");
  #endif
}