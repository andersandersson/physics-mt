#ifndef SOLID_H
#define SOLID_H

typedef struct Solid {
  int x;
  int y;
  int width;
  int height;

  float color;
} Solid;

Solid* solid_create(int x, int y, int width, int height);
void   solid_free(Solid* solid);

#endif // SOLID_H
