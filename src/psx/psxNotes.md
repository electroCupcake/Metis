## Controller
Library: libpad.h

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

To use PadInitDirect() you must for receving:

* PadInitDirect(), and pass parameters that store pad info.
  * Only port 1, port 2.
* OR you can use PadInitMtap() for multitap compatiblity.
* PadStartCom() to start communication.

Transmitting requires:

* PadSetAct(), registers a transmit data buffer.

Interesting Functions:

* PadCheckVsync(), Checks for communication with controller.
* CheckCallback(), determines if program is currently executing in normal or callback context.

## Graphics

Library: libgs.h, libgpu.h, libgte.h

Basic code stubs to deal with graphics
```
// within the BIOS, if the address 0xBFC7FF52 equals 'E', set it as PAL (1). Otherwise, set it as NTSC (0)
switch(*(char *)0xbfc7ff52=='E')
{
  case 'E':
    SetVideoMode(1); 
    break;
  default:
    SetVideoMode(0); 
    break;	
}

GsInitGraph(SCREEN_WIDTH, SCREEN_HEIGHT, GsINTER|GsOFSGPU, 1, 0); // set the graphics mode resolutions. You may also try using 'GsNONINTER' (read LIBOVR46.PDF in PSYQ/DOCS for detailed information)
GsDefDispBuff(0, 0, 0, SCREEN_HEIGHT); // set the top left coordinates of the two buffers in video memory

//functions to call to update display
GsClearDispArea(0,0,0);
DrawPrim(f4);
DrawSync(0); // wait for all drawing to finish
VSync(0); // wait for v_blank interrupt
GsSwapDispBuff(); // flip the double buffers
```

To setup graphics:

* Check if memory address is equal to character E, if its E, set video mode to PAL, otherwise, NTSC.
  * SetVideoMode() is the function is use to set the above.
* GsInitGraph() set the graphics resolution, and various modes.
* GsDefDispBuff() set top left coordinates. Sets both buffers, display, and draw.

To update the display:

* GsClearDispArea(), display area is cleared using IO.
  * GetDrawArea(), GetDrawEnv()... etc looks like there are more complicated ways to draw... dispArea seems to be the wrong choice.
* Draw******() are functions that draw a primitive into the draw buffer.
* DrawSync() waits for all drawing to finish.
* VSynch() wait for v_blank interrupt
* GsSwapDispBuff() flips the double buffers.

## Memory Card