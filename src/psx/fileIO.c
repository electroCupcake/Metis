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

//load a data from CD, returns address pointer in memory.
void loadObjectData(char *p_fname, char **op_data, uint32_t *op_len)
{
  int sizeSectors = 0;
  int numRemain = 0;
  int prevNumRemain = 0;
  u_char result = 0;
  
  DslFILE fileInfo;
  
  if(*op_data != NULL)
  {
    #ifdef DEBUG
      printf("\nINPUT DATA NOT NULL\n");
    #endif
    return;
  }
  
  if(DsSearchFile(&fileInfo, p_fname) <= 0)
  {
    #ifdef DEBUG
      printf("\nFILE SEARCH FAILED\n");
    #endif
    return;
  }

  #ifdef DEBUG
    printf("\nFILE SEARCH SUCCESS\n");
  #endif
    
  sizeSectors = (fileInfo.size + 2047) / 2048;

  #ifdef DEBUG
    printf("\nSECTOR SIZE: %d %d", sizeSectors, fileInfo.size);
  #endif
  
  *op_data = malloc(sizeSectors * 2048);
  
  if(*op_data == NULL)
  {
    #ifdef DEBUG
      printf("\nALLOCATION FAILED\n");
    #endif
    return;
  }
  
  #ifdef DEBUG
    printf("\nMEMORY ALLOCATED\n");
  #endif
  
  DsRead(&fileInfo.pos, sizeSectors, (u_long *)*op_data, DslModeSpeed);
  
  do
  {
    numRemain = DsReadSync(&result);
    
    if(numRemain != prevNumRemain)
    {
      #ifdef DEBUG
	printf("\nNUM REMAIN: %d\n", numRemain);
      #endif
      prevNumRemain = numRemain;
    }
  }
  while(numRemain);
  
  if(op_len != NULL)
  {
    *op_len = fileInfo.size;
  }
  
  #ifdef DEBUG
    printf("\nREAD COMPLETE\n");
  #endif
}
//read from the memory card and return pointer to data
void loadSaveData(char *p_fname, char **op_data, uint32_t len)
{
  long cmds;
  long result;
  
  if(*op_data != NULL)
  {
    #ifdef DEBUG
      printf("\nINPUT DATA NOT NULL\n");
    #endif
    return;
  }
  
  PadStopCom();
  
  *op_data = malloc(len);
  
  if(*op_data == NULL)
  {
    #ifdef DEBUG
      printf("\nMEMORY ALLOCATION FAILED\n");
    #endif
    return;
  }
    
  MemCardInit(1);

  MemCardStart();

  if(MemCardSync(0, &cmds, &result) <= 0)
  {
    #ifdef DEBUG
      printf("\nSync Failed\n");
    #endif
  }

  MemCardAccept(0);

  if(MemCardSync(0, &cmds, &result) <= 0)
  {
    #ifdef DEBUG
      printf("\nSync Failed\n");
    #endif
  }

  if(MemCardOpen(0, p_fname, O_RDONLY) != 0)
  {
    #ifdef DEBUG
      printf("\nOpen Issue\n");
    #endif
    free(*op_data);
    *op_data = NULL;
    return; 
  }

  if(MemCardReadData((unsigned long *)*op_data, 0, len)  != 0)
  {
    #ifdef DEBUG
      printf("\nRead Issue\n");
    #endif
  }

  if(MemCardSync(0, &cmds, &result) <= 0)
  {
    #ifdef DEBUG
      printf("\nSync Failed\n");
    #endif
  }

  MemCardClose();

  MemCardStop();
  
  PadStartCom();
}
//write to the memory card using data passed to it.
void saveSaveData(char *p_fname, char *ip_data, uint32_t len)
{
  long cmds;
  long result;
  
  if(ip_data == NULL)
  {
    #ifdef DEBUG
      printf("\nNULL POINTER PASSED\n");
    #endif
    return;
  }
  
  PadStopCom();
    
  MemCardInit(1);

  MemCardStart();

  if(MemCardSync(0, &cmds, &result) <= 0)
  {
    #ifdef DEBUG
      printf("\nSync Failed\n");
    #endif
  }

  MemCardAccept(0);

  if(MemCardSync(0, &cmds, &result) <= 0)
  {
    #ifdef DEBUG
      printf("\nSync Failed\n");
    #endif
  }

  if(MemCardOpen(0, p_fname, O_WRONLY) != 0)
  {
    #ifdef DEBUG
      printf("\nOpen Issue\n");
    #endif
    return;
  }

  if(MemCardWriteData((unsigned long *)ip_data, 0, len)  != 0)
  {
    #ifdef DEBUG
      printf("\nWrite Issue\n");
    #endif
  }

  if(MemCardSync(0, &cmds, &result) <= 0)
  {
    #ifdef DEBUG
      printf("\nSync Failed\n");
    #endif
  }

  MemCardClose();

  MemCardStop();
  
  PadStartCom();
}