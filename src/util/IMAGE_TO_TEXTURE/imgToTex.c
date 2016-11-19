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
/*
 *
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imgToTex.h"

#define INVALID_PTR	-3
#define INVALID_TYPE	-2
#define INVALID_DATA	-1
#define RAW_DATA	0

#define TIM_HEADER_SIZE    20
#define BMP_HEADER_SIZE    40

#define TIM_HEADER	0x0010
#define BMP_HEADER	0x4D42

#define TIM_4BIT	0x08
#define TIM_8BIT	0x09
#define TIM_16BIT	0x02
#define TIM_24BIT	0x03
#define BMP_16BIT	0x10
#define BMP_24BIT	0x18

#define BMP_COLOR_MASK	0x1F

//helper functions
//detects image type and returns data offset to raw image data and the type
int detectImage(uint8_t const *p_data, int len, uint16_t *op_width, uint16_t *op_height, enum en_imageType *op_type);
//set semiTrans to 1, will set the color specified to semiTransparent, 0 ignores color and sets all semiTrans
//bits back to 0
void setTransBit(uint8_t *op_data, uint8_t red, uint8_t green, uint8_t blue, uint8_t semiTrans, int len);
//swaps rows, as bitmap reverse the bits
//0 success, -1 failure
int reverseData(uint8_t *op_data, int len, int width, int height);


//sets semi trans bit in image data to one, use on raw data
//0 or greater for success, -1 not a valid 16 bit file
int addSemiTrans(uint8_t *op_data, uint8_t red, uint8_t green, uint8_t blue, int len, enum en_imageType type)
{
  switch(type)
  {
    case BITMAP_16:
    case TIM_16:
      setTransBit(op_data, red, green, blue, 1, len);
      break;
    default:
      fprintf(stderr, "\nINVALID TYPE\n");
      return PROCESS_FAILURE;
      break;
  }
  
  return PROCESS_SUCCESS;
}

//clears SemiTransBit, use on raw data
int removeSemiTrans(uint8_t *op_data, int len, enum en_imageType type)
{
  switch(type)
  {
    case BITMAP_16:
    case TIM_16:
      setTransBit(op_data, 0, 0, 0, 0, len);
      break;
    default:
      fprintf(stderr, "\nINVALID TYPE\n");
      return PROCESS_FAILURE;
      break;
  }
  
  return PROCESS_SUCCESS;
}

//convert bitmap data to RAW, if no header detected, or incorrect header, this does nothing.
//RAW data is converted to ABGR (MSB to LSB) from the bmp ARGB (16 bit images only)
int imageToTexture(uint8_t **iop_data, int len, uint16_t *op_width, uint16_t *op_height, enum en_imageType *op_type)
{
  int dataOffset = 0;
  int newLen = 0;
  
  uint8_t *p_temp = NULL;
  
  if(len <= 0)
  {
    fprintf(stderr, "\nLEN FAIL\n");
    return PROCESS_FAILURE;
  }
  
  if(op_width == NULL || op_height == NULL || op_type == NULL)
  {
    fprintf(stderr, "\nPOINTER FAIL\n");
    return PROCESS_FAILURE;
  }
  //this will return the offset, if its not a 16 bit bitmap -1, raw data, 0
  dataOffset = detectImage(*iop_data, len, op_width, op_height, op_type);
  
  if(dataOffset <= 0)
  {
    fprintf(stderr, "\nOFFSET FAIL\n");
    return PROCESS_FAILURE;
  }
  
  newLen = len - dataOffset;
  
  memmove(*iop_data, *iop_data + dataOffset, newLen);
  
  p_temp = realloc(*iop_data, newLen);
  
  if(p_temp != NULL)
  {
    *iop_data = p_temp;
  }
  
  switch(*op_type)
  {
    case BITMAP_24:
    case BITMAP_16:
      reverseData(*iop_data, newLen, *op_width, *op_height);
      break;
    case TIM_24:
    case TIM_16:
      break;
    default:
      fprintf(stderr, "\nINVALID TYPE\n");
      return PROCESS_FAILURE;
      break;
  }
  
  return newLen;
}

int swapRedBlue(uint8_t *op_data, int len, enum en_imageType type)
{
  int index;

  switch(type)
  {
    case BITMAP_16:
    case TIM_16:
      for(index = 0; index < len; index += 2)
      {
	uint8_t top = 0;
	uint8_t middle = 0;
	uint8_t bottom = 0;
	uint8_t semiTransD = 0;
	uint16_t data = 0;

	data = ((op_data[index+1] << 8) | op_data[index]);

	semiTransD = (data & 0x8000) >> 15;
	top = (data & 0x7C00) >> 10;
	middle = (data & 0x03E0) >> 5;
	bottom = (data & 0x001F);

	data = 0;

	data |= (semiTransD << 15) | (bottom << 10) | (middle << 5) | top;

	op_data[index] = data & 0x00FF;

	op_data[index+1] = (data >> 8) & 0x00FF;
      }
      break;
    default:
      fprintf(stderr, "\nINVALID TYPE\n");
      return PROCESS_FAILURE;
      break;
  }
  
  return PROCESS_SUCCESS;
}

//detects bitmap image, if it exists and is of the right type
//this will return the offset, if its not a 16 bit bitmap -1, raw data, 0
int detectImage(uint8_t const *p_data, int len, uint16_t *op_width, uint16_t  *op_height, enum en_imageType *op_type)
{
  int index = 0;
  int dataOffset = 0;
  uint16_t type = 0;
  //not enough data to be at least a TIM, must be larger than the header
  
  if(len < 2)
  {
    return INVALID_DATA;
  }
  
  if(op_width == NULL || op_height == NULL || op_type == NULL)
  {
    return INVALID_PTR;
  }
  
  type = p_data[0] | p_data[1] << 8;
  
  #ifdef DEBUG
    fprintf(stderr, "TYPE: %d TIM: %d BMP: %d\n", type, TIM_HEADER, BMP_HEADER);
  #endif
    
  switch(type)
  {
    case TIM_HEADER:
      if(len < TIM_HEADER_SIZE)
      {
	return INVALID_DATA;
      }
      
      dataOffset = TIM_HEADER_SIZE;
      
      switch(p_data[4])
      {
	//add 24 bit tims later
	case TIM_4BIT:
	  *op_type = TIM_4;
	  //add 4 bit types later
	  break;
	case TIM_8BIT:
	  *op_type = TIM_8;
	  //add 8 bit types later
	  break;
	case TIM_16BIT:
	  *op_type = TIM_16;
	  *op_width = p_data[16] | (p_data[17] << 8);
	  *op_height = p_data[18] | (p_data[19] << 8);
	  break;
	default:
	  *op_type = INVALID;
	  return INVALID_TYPE;
	  break;
      }
      
      break;
    case BMP_HEADER:
      if(len < BMP_HEADER_SIZE)
      {
	return INVALID_DATA;
      }
      
      dataOffset = p_data[10] | (p_data[11] << 8) | (p_data[12] << 16) | (p_data[13] << 24);
      
      switch(p_data[28])
      {
	//add 24 bit bitmaps
	case BMP_16BIT:
	  *op_type = BITMAP_16;
	  break;
	default:
	  *op_type = INVALID;
	  return INVALID_TYPE;
	  break;
      }
      
      *op_width = p_data[18] | (p_data[19] << 8) | (p_data[20] << 16) | (p_data[21] << 24); 
      
      *op_height = p_data[22] | (p_data[23] << 8) | (p_data[24] << 16) | (p_data[25] << 24);
      
      break;
    default:
      fprintf(stderr, "\nINVALID TYPE\n");
      *op_type = INVALID;
      return INVALID_TYPE;
      break;
  }
  
  #ifdef DEBUG
    fprintf(stderr, "OFFSET %d, WIDTH %d, HEIGHT %d\n", dataOffset, *op_width, *op_height);
  #endif
  
  return dataOffset;
}

//set semiTrans to 1, will set the color specified to semiTransparent, 0 ignores color and sets all semiTrans
//bits back to 0
void setTransBit(uint8_t *op_data, uint8_t red, uint8_t green, uint8_t blue, uint8_t semiTrans, int len)
{
  int index;
  
  for(index = 0; index < len; index += 2)
  {
    uint8_t redD = 0;
    uint8_t greenD = 0;
    uint8_t blueD = 0;
    uint16_t data = 0;

    data = ((op_data[index+1] << 8) | op_data[index]);
    
    blueD = (data & 0x7C00) >> 10;
    greenD = (data & 0x03E0) >> 5;
    redD = (data & 0x001F);

    if((semiTrans == 0) || ((red & BMP_COLOR_MASK) == redD) && ((blue & BMP_COLOR_MASK) == blueD) && ((green & BMP_COLOR_MASK) == greenD))
    {
      op_data[index+1] |= (semiTrans & 0x01) << 7;
      op_data[index+1] &= (((semiTrans & 0x01) << 7) | 0x7F);
    }
  }
}

int reverseData(uint8_t *op_data, int len, int width, int height)
{
  int index;
  
  if(len <= 0)
  {
    return PROCESS_FAILURE;
  }
  
  for(index = 0; index < height/2; index++)
  {
    uint8_t tempData[width * 2];
    
    memcpy(tempData, &op_data[index * width * 2], width * 2);
    
    memcpy(&op_data[index * width * 2], &op_data[len - ((index+1) * width * 2)], width * 2);
    
    memcpy(&op_data[len - ((index+1) * width * 2)], tempData, width * 2);
  }
  
  return PROCESS_SUCCESS;
}