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

#include "testEngine.h"

enum inputTypes g_decodedInput = NONE;

//get the input and reset it to NONE
enum inputTypes getInput()
{
  enum inputTypes temp = g_decodedInput;
  g_decodedInput = NONE;
  return temp;
}
