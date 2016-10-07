#PSX Low Level Notes

## Controller
####Library: libpad.h

Below is the way data is stored for the controller using PadInitDirect().

```css
struct
{
  u_char status;
  u_char type:4;
  u_char recvSize:4;

  union
  {
    struct
    {
      u_char left:1;
      u_char down:1;
      u_char right:1;
      u_char up:1;
      u_char start:1;
      u_char NA1:1;
      u_char NA2:1;
      u_char select:1;
    } bit;
  u_char byte;

  }first;

  union
  {
    struct
    {
      u_char square:1;
      u_char ex:1;
      u_char circle:1;
      u_char triangle:1;
      u_char R1:1;
      u_char L1:1;
      u_char R2:1;
      u_char L2:1;
    } bit;
  u_char byte;

  }second;
} g_pad0;
```

#### To use PadInitDirect() you must for receving:

* PadInitDirect(), and pass parameters that store pad info.
  * Only port 1, port 2.
* OR you can use PadInitMtap() for multitap compatiblity.
* PadStartCom() to start communication.

#### Transmitting requires:

* PadSetAct(), registers a transmit data buffer.

#### Interesting Functions:

* PadCheckVsync(), Checks for communication with controller.
* CheckCallback(), determines if program is currently executing in normal or callback context.

## Graphics

#### Library: libgs.h, libgpu.h, libgte.h

Basic code stubs to deal with graphics
```
switch(*(char *)0xbfc7ff52=='E')
{
  case 'E':
    SetVideoMode(1); 
    break;
  default:
    SetVideoMode(0); 
    break;	
}

GsInitGraph(SCREEN_WIDTH, SCREEN_HEIGHT, GsINTER|GsOFSGPU, 1, 0);
GsDefDispBuff(0, 0, 0, SCREEN_HEIGHT);

//functions to call to update display
GsClearDispArea(0,0,0);
DrawPrim(f4);
DrawSync(0); // wait for all drawing to finish
VSync(0); // wait for v_blank interrupt
GsSwapDispBuff(); // flip the double buffers
```

#### To setup graphics:

* Check if memory address is equal to character E, if its E, set video mode to PAL, otherwise, NTSC.
  * SetVideoMode() is the function is use to set the above.
* GsInitGraph() set the graphics resolution, and various modes.
* GsDefDispBuff() set top left coordinates. Sets both buffers, display, and draw.

#### To update the display:

* GsClearDispArea(), display area is cleared using IO.
  * GetDrawArea(), GetDrawEnv()... etc looks like there are more complicated ways to draw... dispArea seems to be the wrong choice.
* Draw******() are functions that draw a primitive into the draw buffer.
* DrawSync() waits for all drawing to finish.
* VSynch() wait for v_blank interrupt
* GsSwapDispBuff() flips the double buffers.

### Without using extended graphics library

#### Display environment:

* SetDefDispEnv() to set fields of DISPENV struct.
* PutDispEnv() to make a display environment current.
* GetDispEnv() gets a pointer to the current display environment.

#### Draw environment:

* SetDefDrawEnv() to set fields in DRAWENV struct.
* PutDrawEnv() to make a draw environment current.
* GetDrawEnv() gets a pointer to the current drawing environment.

#### Code for switching between double buffers.
```
DRAWENV drawenv[2]; 	 	/*drawing environments*/
DISPENV dispenv[2];			/*display environments*/
int dispid = 0;    	 		/*display buffer ID*/
while (1) {
  VSync(0);			        /*wait for vertical blank*/
  dispid = (dispid + 1) %2;	/*toggle buffer ID between 0 and 1*/
  PutDrawEnv(&drawenv[dispid]);	/*switch drawing environment*/
  PutDispEnv(&dispenv[dispid]);	/*switch display environment*/
}
```

#### Sync and Reset:

* ResetGraph() resets the graphics system.
	* Has 3 levels:
		* Level 0: Complete reset of graphics system.
		* Level 1: Cancels current commands and clears queue.
		* Level 3: Same as 0, but the draw and display environments are preserved.
* Sync has two methods:
	* Polling:
		* DrawSync() can block till drawing queue done, or return number of positions in queue.
		* VSync() deteck the next vertical blank period.
	* Callbacks:
		* VSyncCallback() set a callback function to call at beginning of vertical blank period.
		* DrawSyncCallback() set a callback function that is run when a non-blocking drawing operation completes

#### Packet Double Buffer:

* Ordering table (OT) used as packet buffer. 
* CPU registers OT, GPU draws.
*  One OT for registering, the other for drawing, double buffer setup.
	* Example in run-time library over view makes little sense, since in loops over the register, then draws. Blocking vs non-blocking?

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


#### Primitive Rendering Speed

* Check for bottleneck at CPU or GPU
* DrawSync(), immediate return, bottleneck at CPU, else at GPU (measure latency of GPU).

## Memory Card

#### Library: libcard.h

#### Specifications

* Capacity, 120 Kbytes in 128-byte sectors
* Synchronous serial communication
* No access for 20 ms after reading 1 sector
* Max continuous reading speed is 10 Kbyte/sec
* basic library, low level library.

##### Functions

* InitCARD(), Initializes the memory card bios and enters idle state. Can set if shared with controller or not.
* StartCARD(), Changes memory Card BIOS to run state (initCARD must be called first!).
* StopCARD(), Sets memory card BIOS to idle state. Stops game pad as well.
* _bu_init(), Initialize filesystem, as this does not happen automatically (unformatted?, initCARD must be called first).
* _card_format(), format memory card at specified port.
* _card_info(), get card info at specified port.
* _card_load(), Reads file management info, at a specified port, must be called once before using open().
* _card_read(), read one block from the memory card (128 bytes, one sector), at a specified port.
* _card_write(), same as above, except it writes a block.

#### EXTENDED Memory Card Library

#### Library: libmcrd.h

* High-Level library for the memory card
	* Can check for the memory card.
	* Red, write, delete and create files.
	* Directory Info
	* Formatting.
	* Check state.

##### Functions

* MemCardAccept(), Test connection, if connected gets more info. Must be called first, and only again if a card is swapped
* MemCardClose(), Closes file that was opened by MemCardOpen.
* MemCardCreateFile(), Creates a file on the memory card, with a specified name.
* MemCardDeleteFile(), Deletes file of a specific name.
* MemCardEnd(), Terminates system, MemCardStop must be called first is MemCardStart has been called.
* MemCardExist(), Checks for the presence of a card.
* MemCardFormat(), Format a memory card (full format).
* MemCardGetDirentry(), Finds files in a specified directory
* MemCardInit(), Initializes memory card system, must be called first after InitPAD,GUN,TAP and not called twice.
* MemCardOpen(), Opens a file from a specified memory card.
* MemCardReadData(),  Reads data from file setup by open, from the memory card in 128 byte chunks (bytes must be a multiple of 128).
* MemCardReadFile(), Reads data from a specified memory card file. Same byte requirement as above.
* MemCardStart(), places memory card system in active state after MemoryCardInit has been called.
* MemCardStop(), stops memory card system started my MemCardStart().
* MemCardSync(), Sync memory card access by waiting for termination of asynchronous functions calls.
* MemCardWriteData(), Writes data to a file setup by open, to the memory card in 128 byte chunks (bytes must be a multiple of 128).
* MemCardWriteFile(), Write data to a specified memory card file. Same byte requirement as above.

#### Writing to a Memory Card

```
MemCardInit(1);
	
MemCardStart();

if(MemCardSync(0, &cmds, &result) <= 0)
{
  FntPrint("START WTF\n\n");
}

MemCardAccept(0);

if(MemCardSync(0, &cmds, &result) <= 0)
{
  FntPrint("ACCEPT WTF\n\n");
}

if(MemCardCreateFile(0, "test", 1) != 0)
{
  FntPrint("FILE CREATE WTF\n\n");
}

if(MemCardOpen(0, "test", O_WRONLY) != 0)
{
  FntPrint("Open WTF\n\n");
}

if(MemCardWriteData((unsigned long *)&(*phrase), 0, 128)  != 0)
{
  FntPrint("Write WTF\n\n");
}

if(MemCardSync(0, &cmds, &result) <= 0)
{
  FntPrint("WRITE WTF\n\n");
}

MemCardClose();

MemCardStop();
```

#### Reading from a Memory Card
```
MemCardInit(1);

MemCardStart();

if(MemCardSync(0, &cmds, &result) <= 0)
{
  FntPrint("START WTF\n\n");
}

MemCardAccept(0);

if(MemCardSync(0, &cmds, &result) <= 0)
{
  FntPrint("ACCEPT WTF\n\n");
}

if(MemCardCreateFile(0, "test", 1) != 0)
{
  FntPrint("FILE CREATE WTF\n\n");
}

if(MemCardOpen(0, "test", O_WRONLY) != 0)
{
  FntPrint("Open WTF\n\n");
}

if(MemCardWriteData((unsigned long *)&(*phrase), 0, 128)  != 0)
{
  FntPrint("Write WTF\n\n");
}

if(MemCardSync(0, &cmds, &result) <= 0)
{
  FntPrint("WRITE WTF\n\n");
}

MemCardClose();

MemCardStop();
```