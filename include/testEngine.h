/*
 * Metis
 *
 *  Created on: Sep 17, 2016
 *      Author: John Convertino
 *		email: electrobs@gmail.com
 *		
 *		DESCRIPTION
 *
 *		Version: ADD_HERE
 *		Sep 17, 2016 *VERSION*	*NOTES*
 *
 *		TODO
 */

#ifndef TESTENGINE_H_
#define TESTENGINE_H_

#include <stdlib.h>

//includes based on platform
#ifdef psx
  #include <libgte.h>
  #include <libgpu.h>
  #include <libgs.h>
  #include <libpad.h>
  #include <libetc.h>
#else
  #include <unistd.h>
  #include <stdio.h>
  #include <GL/glew.h>
  #include <GLFW/glfw3.h>
#endif

//size of the square on screen, will always be 50 pixels of the total screen size
#define SQUARE_SIZE	50
//screen defines based on platform
#ifdef psx
  #define SCREEN_HEIGHT	240
  #define SCREEN_WIDTH	320
#else
  #define SCREEN_HEIGHT	480
  #define SCREEN_WIDTH	640
  #define WINDOW_NAME	"MOVING SQUARE"
#endif

//define polyF4 for openGL, this is the struct the psx uses
struct s_polyF4 {
  unsigned long *tag;
  unsigned char r0, g0, b0;
  unsigned char code;
  short x0,y0;
  short x1,y1;
  short x2,y2;
  short x3,y3;
};
//global game pad buffer, this is updated by a callback buried in the psyq library
#ifdef psx
  static u_char g_gamePad[2][34];
#endif

//to help translate input into a common type
enum inputTypes {UP, DOWN, LEFT, RIGHT, CHANGE_COLOR, NONE};
extern enum inputTypes g_decodedInput;

//prototypes
//initializes graphics
void *initGraphics();
//initialize input
void initInput(void *window);
//get translated input
enum inputTypes getInput();
//move the primitive passed based on input from keyboard or controller
void movePrimitive(struct s_polyF4 *f4, enum inputTypes decodedInput);
//generate primite, a square in this case
struct s_polyF4 genPrimitive();
//update display window with primitive
void display(void *window, struct s_polyF4 *f4);
//check if the display is still live (psx is infinite)
int displayAlive(void *window);
//cleanup for glfw, does nothing for psx
void cleanup(void *window);

#ifdef psx
  //create a callback based on the vsync to translate input
  static void inputCallback();
#else
  //define a DramPrim for linux using openGL (already defined for psx)
  void DrawPrim(struct s_polyF4 *f4);
  //callback to decode input
  static void inputCallback(GLFWwindow *window, int key, int scanCode, int action, int mods);
#endif


#endif /* TESTENGINE_H_ */
