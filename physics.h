#ifndef PHYSICS_H
#define PHYSICS_H

#include "list.h"
#include "solid.h"

void physics_init(int threads);
void physics_tick();
void physics_terminate();

void physics_add(Solid* item);
void physics_remove(Solid* item);

#endif // PHYSICS_H
