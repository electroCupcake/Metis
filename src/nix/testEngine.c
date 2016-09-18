/*
 * Metis
 *
 *  Created on: Sep 17, 2016
 *      Author: John Convertino
 *		email: electrobs@gmail.com
 *		
 *		Linux graphics routines for test program
 *
 *		Version: v0.1
 *		Sep 17, 2016  v0.1	First split
 *
 *		TODO
 */

#include "testEngine.h"

//initialize graphics
void *initGraphics()
{
  //glfw init
  int returnValue = 0;
  GLenum glewReturnValue = 0;
  GLFWwindow *window = NULL;

  returnValue = glfwInit();

  if(returnValue == GL_FALSE)
  {
    fprintf(stderr, "Failed to initialize GLFW.\n");
    exit(EXIT_FAILURE);
  }

  window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_NAME, NULL, NULL);

  if(window == NULL)
  {
    fprintf(stderr, "Failed to create window\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  //vsync?
  glfwSwapInterval(1);

  glewReturnValue = glewInit();

  if(glewReturnValue != GLEW_OK)
  {
    fprintf(stderr, "Failed to initialize GLEW... %s\n", glewGetErrorString(glewReturnValue));
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  //setup area using openGL calls
  //set background color
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //change drawing area to one that matches the playstation
  glOrtho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, -1.0f);
  //used to time the color change button press (set to allow initial press to go regardless of time)
  glfwSetTime(1.0f);

  return window;
}

//setup input
void initInput(void *window)
{
  //glfw
  if(window != NULL)
  {
    glfwSetKeyCallback((GLFWwindow *)window, inputCallback);
  }
}

//universal method of moving the primitive on screen (at this point does a warp to the other side if you touch it)
void movePrimitive(struct s_polyF4 *f4, enum inputTypes decodedInput)
{
  switch(decodedInput)
  {
    case UP:
      f4->y0--;
      f4->y1--;
      f4->y2--;
      f4->y3--;
      if(f4->y0 < 0)
      {
	f4->y0 = SCREEN_HEIGHT - SQUARE_SIZE;
	f4->y1 = SCREEN_HEIGHT - SQUARE_SIZE;
	f4->y2 = SCREEN_HEIGHT;
	f4->y3 = SCREEN_HEIGHT;
      }
      break;
    case DOWN:
      f4->y0++;
      f4->y1++;
      f4->y2++;
      f4->y3++;
      if(f4->y2 > SCREEN_HEIGHT)
      {
	f4->y0 = 0;
	f4->y1 = 0;
	f4->y2 = SQUARE_SIZE;
	f4->y3 = SQUARE_SIZE;
      }
      break;
    case LEFT:
      f4->x0--;
      f4->x1--;
      f4->x2--;
      f4->x3--;
      if(f4->x0 < 0)
      {
	f4->x0 = SCREEN_WIDTH - SQUARE_SIZE;
	f4->x1 = SCREEN_WIDTH;
	f4->x2 = SCREEN_WIDTH - SQUARE_SIZE;
	f4->x3 = SCREEN_WIDTH;
      }
      break;
    case RIGHT:
      f4->x0++;
      f4->x1++;
      f4->x2++;
      f4->x3++;
      if(f4->x1 > SCREEN_WIDTH)
      {
	f4->x0 = 0;
	f4->x1 = SQUARE_SIZE;
	f4->x2 = 0;
	f4->x3 = SQUARE_SIZE;
      }
      break;
    case CHANGE_COLOR:
      //check if it has been greater than 1 second since glfw time has been reset
      if(glfwGetTime() > 1.0f)
      {
	glfwSetTime(0.0f);
	f4->r0 = rand()%256;
	f4->g0 = rand()%256;
	f4->b0 = rand()%256;
      }
      break;
    default:
      break;
  }
}

//generate primitive
struct s_polyF4 genPrimitive()
{
  struct s_polyF4 temp;

  temp.r0 = 0;
  temp.g0 = 0;
  temp.b0 = 255;

  temp.x0 = 0;
  temp.y0 = 0;
  temp.x1 = SQUARE_SIZE;
  temp.y1 = 0;
  temp.x2 = 0;
  temp.y2 = SQUARE_SIZE;
  temp.x3 = SQUARE_SIZE;
  temp.y3 = SQUARE_SIZE;

  return temp;

}

//display primitive, window is only used for glfw
void display(void *window, struct s_polyF4 *f4)
{
  //GLFW
  //draw primitive function based on openGL calls
  DrawPrim(f4);
  if(window != NULL)
  {
    //swap draw and display buffer (back and front buffer)
    glfwSwapBuffers((GLFWwindow *)window);
    //poll events in the winow system
    glfwPollEvents();
  }
}

//only needed for glfw, does nothing for the psx
void cleanup(void *window)
{
  if(window != NULL)
  {
    glfwDestroyWindow((GLFWwindow *)window);
  }
}

//check if the window is still open, since the PSX does not have a window, it returns a 1 to run infinitely
int displayAlive(void *window)
{
  return (glfwWindowShouldClose((GLFWwindow *)window) == 0 ? 1 : 0);
}

//openGL DrawPrim function, mimics what the PSX version does (theirs is native aka, build in)
void DrawPrim(struct s_polyF4 *f4)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3ub(f4->r0, f4->g0, f4->b0);
  glBegin(GL_QUADS);
  glVertex2i(f4->x0, f4->y0);
  glVertex2i(f4->x1, f4->y1);
  glVertex2i(f4->x3, f4->y3);
  glVertex2i(f4->x2, f4->y2);
  glEnd();
  glFlush();
}

//keyboard input callback to decode key pressed into are decoded type.
static void inputCallback(GLFWwindow *window, int key, int scanCode, int action, int mods)
{
  if(action == GLFW_PRESS || action == GLFW_REPEAT)
  {
    switch(key)
    {
      case GLFW_KEY_UP:
	g_decodedInput = UP;
	break;
      case GLFW_KEY_DOWN:
	g_decodedInput = DOWN;
	break;
      case GLFW_KEY_LEFT:
	g_decodedInput = LEFT;
	break;
      case GLFW_KEY_RIGHT:
	g_decodedInput = RIGHT;
	break;
      case GLFW_KEY_X:
	g_decodedInput = CHANGE_COLOR;
	break;
      default:
	g_decodedInput = NONE;
	break;
    }
  }
}

