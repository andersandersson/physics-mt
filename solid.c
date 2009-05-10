#include "solid.h"

#include <stdlib.h>


Solid* solid_create(int x, int y, int width, int height) {
  Solid* solid = malloc(sizeof(Solid));

  solid->x = x;
  solid->y = y;
  solid->width = width;
  solid->height = height;

  return solid;
}

void solid_free(Solid* solid) {
  free(solid);
}
