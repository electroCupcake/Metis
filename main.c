/*
 * PROJECT-GLFW-PSX-MOVING_SQUARE
 *
 *  Created on: Sep 5, 2016
 *      Author: John Convertino
 *		email: electrobs@gmail.com
 *
 *		Linux and PlayStation cross platform test application.
 *		This application is just a square that can move either via
 *		keyboard in linux (UP, DOWN, LEFT, RIGHT, x for color change)
 *		or via how ever the pcsx emulator is mapped for d-pad directions
 *		and the x(cross) button (color change).
 *
 *		Version: 0.2
 *              Sep 18, 2016 0.2        Split into seprate files for cmake tests.
 *		Sep 5, 2016  0.1	First version
 *
 *		TODO
 */

#include "testEngine.h"

//application, takes no arguments at this point
int main(int argc, char *argv[])
{
  struct s_polyF4 polyF4;

  void *window = initGraphics();

  initInput(window);

  polyF4 = genPrimitive();

  while(displayAlive(window))
  {
    movePrimitive(&polyF4, getInput());
    display(window, &polyF4);
  }

  cleanup(window);

  return 0;
}
