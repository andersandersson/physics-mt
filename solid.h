#ifndef SOLID_H
#define SOLID_H

#include <GL/glfw.h>

typedef struct Solid {
  int x;
  int y;
  int width;
  int height;

  int v_x;
  int v_y;

  int new_x;
  int new_y;

  float color;
  float theta;

  GLFWmutex lock;
} Solid;

Solid* solid_create(int x, int y, int width, int height);
void   solid_set_velocity(Solid* solid, int vx, int vy);
void   solid_free(Solid* solid);

#endif // SOLID_H
