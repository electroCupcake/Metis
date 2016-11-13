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

#ifndef _IMGTOTEX_H
#define _IMGTOTEX_H

#ifndef PROCESS_SUCCESS
  #define PROCESS_SUCCESS	0
#endif

#ifndef PROCESS_FAILURE
  #define PROCESS_FAILURE	-1
#endif

enum en_imageType {BITMAP_16, TIM_4, TIM_8, TIM_16, INVALID};

//sets semi trans bit in image data to one, use on raw data 16bit (texture data) only
//0 or greater success, -1 failure
int addSemiTrans(uint8_t *iop_data, uint8_t red, uint8_t green, uint8_t blue, int len, enum en_imageType type);

//clears SemiTransBit, use on raw data 16bit (texture data) only
//0 or greater success, -1 failure
int removeSemiTrans(uint8_t *iop_data, int len, enum en_imageType type);

//convert TIM, or BITMAP data into RAW data, detects the type and returns in and the new size of the data
//only works with 16 bit images at the moment (no clut or 24 bit support)
//will eventually also return clut data for 4 and 8 bit types
int imageToTexture(uint8_t **iop_data, int len, uint16_t *op_width, uint16_t *op_height, enum en_imageType *op_type);

//convert rgb data to bgr data, works with raw 16bit data only
//0 or greater success, -1 failure
int swapRedBlue(uint8_t *iop_data, int len, enum en_imageType type);

#endif