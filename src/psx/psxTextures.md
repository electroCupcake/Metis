#PSX Texture Notes

## Textures

#### Library: libgs.h, libgpu.h, libgte.h

#### Example: textureTest, textureTestCD

### Utilitys

#### Conversion

* TIMUTIL.EXE = Converts bitmap (24 bit, no color space info) images to TIM images.
* TIMTOOL.EXE = Check location of TIM image in PlayStation video memory.
* bin2h.exe = Converts TIM images to array of 8 bit values. 

#### Texture Mapping:

* Textures, 2D images mapped onto surfaces.
* Stored in areas of the frame buffer called texture pages.
* Pages are 256 x 256 bitmaps.
* Upper left X coordinate must be a multiple of 64, and Y a multiple of 256 (in frame buffer).
* Three Pixel formats, each primitive can have its own.
	* 4 bit, clut-based, 16 colors, 64 wide.
	* 8 bit, clut-based, 256 colors, 128 wide
		* CLUT is a Color lookup table, also known as a palette.
	* 16 bit, Direct RGB, 32767 colors, 256 wide. (15 bit RGB, 1 semi-transparent status bit). 
* Can be done with primitives and sprites, but sprites do not specify a texture, and must have the current texture page set.

##### Functions

* LoadImage(), transfer data to a frame buffer from memory.
* MoveImage(), transfer data between two places in the frame buffer.
* StoreImage(), transer data from frame buffer to memory.
* GetTpage(), calculates texture page ID and returns it for use.
* GetClut(), use to process color lookup table.

### Example Code


##### Loading TIM from CD
```
DslFILE fileInfo;
int sizeSectors = 0;

	
u_long *image = NULL;

//CD init 
DsInit();

if(DsSearchFile(&fileInfo, "\\TIM\\YAKKO.TIM;1") <= 0)
{
  printf("\nFILE READ FAILED\n");
}
else
{
  printf("\nFILE READ SUCCESS\n");
}

sizeSectors = (fileInfo.size + 2047) / 2048;


printf("\nSECTOR SIZE: %d %d", sizeSectors, fileInfo.size);

image = malloc(sizeSectors * 2048);

if(image == NULL)
{
  printf("\nALLOCATION FAILED\n");
}
else
{
  printf("\nMEMORY ALLOCATED\n");
}

DsRead(&fileInfo.pos, sizeSectors, image, DslModeSpeed);

do
{
  numRemain = DsReadSync(&result);
  
  if(numRemain != prevNumRemain)
  {
    printf("\nNUM REMAIN: %d\n", numRemain);
    prevNumRemain = numRemain;
  }
}
while(numRemain);


printf("\nREAD COMPLETE\n");

DsClose();
```

##### Loading a TIM image already in memory.

```
POLY_FT4 f4;

//setup textured primitive
SetPolyFT4(&f4);
setRGB0(&f4, 128, 128, 128);

//add image to primitive
GsGetTimInfo((u_long *)(image+4), &g_timData);

//load texture page, and return its id to the textured primitve
f4.tpage = LoadTPage(g_timData.pixel, g_timData.pmode, 0, g_timData.px, g_timData.py, g_timData.pw, g_timData.ph);

//load clut, and return its id to the texture primitive
f4.clut = LoadClut(g_timData.clut, g_timData.cx, g_timData.cy);

//set primitive coordinates, and set its texture coordinates
setXY4(&f4, 0, 0, 50, 0, 0, 50, 50, 50);
setUV4(&f4, 0, 0, 50, 0, 0, 50, 50, 50);
```