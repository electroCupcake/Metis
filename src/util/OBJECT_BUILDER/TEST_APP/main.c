 /*
  * 
  * Test program for image to texture
  * 
  */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "objectBuilder.h"

#define ONE_MB (1 << 20)

int main()
{
  int len = 0;
  uint16_t width = 0;
  uint16_t height = 0;
  
  struct s_object *p_object = NULL;

  FILE *fdREAD = NULL;
  
  uint8_t *p_buffer = NULL;

  fdREAD = fopen("EXAMPLE.XML", "rb");

  if(fdREAD == NULL)
  {
    perror("File open fail");
    return EXIT_FAILURE;
  }

  p_buffer = malloc(ONE_MB);
  
  initObjectBuilder();
  
  do
  {
    len = fread(p_buffer, sizeof(uint8_t), ONE_MB, fdREAD);

    p_object = getObject(p_buffer);
    
    if(p_object != NULL)
    {
      printf("\n\nprimType: %d\n", p_object->primType);
      printf("objectType: %d\n", p_object->objectType);
      printf("\nLOCAL\n");
      printf("\n       VERTEX 0: %d %d %d VERTEX 1: %d %d %d VERTEX 2: %d %d %d VERTEX 3: %d %d %d\n"
	, p_object->local.vertex0.vx, p_object->local.vertex0.vy, p_object->local.vertex0.vz
	, p_object->local.vertex1.vx, p_object->local.vertex1.vy, p_object->local.vertex1.vz
	, p_object->local.vertex2.vx, p_object->local.vertex2.vy, p_object->local.vertex2.vz
	, p_object->local.vertex3.vx, p_object->local.vertex3.vy, p_object->local.vertex3.vz
      );
      printf("\n       COLOR  0: %d %d %d COLOR  1: %d %d %d COLOR  2: %d %d %d COLOR  3: %d %d %d\n"
	, p_object->local.color0.r, p_object->local.color0.g, p_object->local.color0.b
	, p_object->local.color1.r, p_object->local.color1.g, p_object->local.color1.b
	, p_object->local.color2.r, p_object->local.color2.g, p_object->local.color2.b
	, p_object->local.color3.r, p_object->local.color3.g, p_object->local.color3.b
      );
      printf("\nTEXTURE\n");
      printf("\n       ID: %d SIZE: %d VERTEX 0: %d %d %d VRAM: %d %d %d DIMENSIONS: %d %d FILE %s\n"
	, p_object->local.p_texture->id, p_object->local.p_texture->size
	, p_object->local.p_texture->vertex0.vx, p_object->local.p_texture->vertex0.vz, p_object->local.p_texture->vertex0.vy
	, p_object->local.p_texture->vramVertex.vx, p_object->local.p_texture->vramVertex.vy, p_object->local.p_texture->vramVertex.vz
	, p_object->local.p_texture->dimensions.w, p_object->local.p_texture->dimensions.h
	, p_object->local.p_texture->file
      );
      printf("\nWORLD\n");
      printf("\n       TRANSCOOR: %d %d %d SCALECOOR: %d %d %d ROTCOOR: %d %d %d\n\n\n"
	, p_object->world.transCoor.vx, p_object->world.transCoor.vy, p_object->world.transCoor.vz
	, p_object->world.scaleCoor.vx, p_object->world.scaleCoor.vy, p_object->world.scaleCoor.vz
	, p_object->world.rotCoor.vx, p_object->world.rotCoor.vy, p_object->world.rotCoor.vz
      );
    }
  }
  while(len > 0 && feof(fdREAD) == 0);

  freeObject(&p_object);

  return EXIT_SUCCESS;
}