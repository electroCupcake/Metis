 /*
  * 
  * Test program for image to texture
  * 
  */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "imgToTex.h"

#define ONE_MB (1 << 20)

int main()
{
  int len = 0;
  uint16_t width = 0;
  uint16_t height = 0;
  
  enum en_imageType imageType = 0;

  FILE *fdREAD = NULL;
  FILE *fdWRITE = NULL;
  uint8_t *p_buffer = NULL;

  fdREAD = fopen("RED16X.TIM", "rb");

  if(fdREAD == NULL)
  {
    perror("File open fail");
    return EXIT_FAILURE;
  }

  fdWRITE = fopen("REDSWAP.data", "wb");

  if(fdWRITE == NULL)
  {
    perror("File open fail");
    return EXIT_FAILURE;
  }

  p_buffer = malloc(ONE_MB);

  do
  {
    len = fread(p_buffer, sizeof(uint8_t), ONE_MB, fdREAD);

    len = imageToTexture(&p_buffer, len, &width, &height, &imageType);
    
    printf("LEN: %d WIDTH %d HEIGHT %d TYPE %d\n", len, width, height, imageType);

//     if(swapRedBlue(p_buffer, len, imageType) < 0)
//     {
//       printf("SWAP FAILED\n");
//     }
       
    if(addSemiTrans(p_buffer, 255, 0, 0, len, imageType) < 0)
    {
      printf("FAILED TO SET SEMITRANS\n");
    }
    

    len = fwrite(p_buffer, sizeof(uint8_t), len, fdWRITE);
  }
  while(len > 0 && feof(fdREAD) == 0);


  return EXIT_SUCCESS;
}