#include "solid.h"

#include <stdlib.h>


Solid* solid_create(int x, int y, int width, int height) {
  Solid* solid = malloc(sizeof(Solid));

  solid->x = x;
  solid->y = y;
  solid->width = width;
  solid->height = height;
  solid->theta = 0.0f;

  solid->lock = glfwCreateMutex();

  return solid;
}

void solid_set_velocity(Solid* solid, int vx, int vy) {
  solid->v_x = solid->x; //vx;
  solid->v_y = solid->y;//vy;
}

void solid_free(Solid* solid) {
  free(solid);
}
