/*
 * Metis
 *
 *  Created on: Sep 17, 2016
 *      Author: John Convertino
 *		email: electrobs@gmail.com
 *		
 *		Common Code for all arch
 *
 *		Version: v0.1
 *		Sep 17, 2016 v0.1	First Split
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
