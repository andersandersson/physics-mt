#include <GL/glfw.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "physics.h"
#include "thread.h"
#include "list.h"

typedef struct PhysicsData {
  int     length;
  List*   solid_list;
  List*   collision_list;
  Thread* thread;
} PhysicsData;


int g_num_threads = 1;
PhysicsData* world = NULL;

void physics_init(int threads) {
  int i;
  char name[1024];
  g_num_threads = threads;

  world = malloc(g_num_threads*sizeof(PhysicsData));

  for(i=0; i<g_num_threads; i++) {
    sprintf(name, "[Thread %d]", i);
    
    world[i].solid_list = list_create();
    world[i].collision_list = list_create();
    world[i].thread = thread_create(name);
    world[i].length = 0;
  }
}

void physics_add(Solid* solid) {
  int min = world[0].length;
  int index = 0;
  int i = 0;

  for(i=0; i<g_num_threads; i++) {
    if(world[i].length < min) {
      index = i;
      min = world[i].length;
    }
  }

  world[index].length++;
  list_push(world[index].solid_list, solid);
}

void physics_update(void* arg) {
  PhysicsData* data = (PhysicsData*) arg;
  List* list = data->solid_list;
  Node* node;
  Solid* solid;

  if(NULL != list) {
    node = list->first_node;
  } else {
    node = NULL;
  }
  
  while(NULL != node) {
    solid = (Solid*) node->item;

    solid->theta += 0.1;
    solid->x = (float)solid->v_x + 20.0*cosf(solid->theta);
    solid->y = (float)solid->v_y + 20.0*sinf(solid->theta);
    
    node = node->next;
  } 
}

void physics_detect_collisions(void* arg) {
  PhysicsData* data = (PhysicsData*) arg;
  List* list = data->solid_list;
  List* collisions = data->collision_list;
  Node* node;
  Solid* solid;

  if(NULL != list) {
    node = list->first_node;
  } else {
    node = NULL;
  }
  
  while(NULL != node) {
    solid = (Solid*) node->item;

    if(solid->x < 0 || solid->x > (640-solid->width)) {
      list_push(collisions, solid);
    }

    if(solid->y < 0 || solid->y > (480-solid->height)) {
      list_push(collisions, solid);
    }

    node = node->next;
  } 
}

void physics_handle_collisions(void* arg) {
  PhysicsData* data = (PhysicsData*) arg;
  List* collisions = data->collision_list;
  Solid* solid;

  solid = list_pop(collisions);
  
  while(NULL != solid) {
    glfwLockMutex(solid->lock);

    if(solid->x < 0) {
      solid->x = 0;
      solid->v_x = solid->v_x * -1;
    }

    if(solid->x > 640 - solid->width) {
      solid->x = 640 - solid->width;
      solid->v_x = solid->v_x * -1;
    }

    if(solid->y < 0) {
      solid->y = 0;
      solid->v_y = solid->v_y * -1;
    }

    if(solid->y > 480 - solid->height) {
      solid->y = 480 - solid->height;
      solid->v_y = solid->v_y * -1;
    }

    glfwUnlockMutex(solid->lock);
    solid = list_pop(collisions);
  }
}

void physics_tick() {
  int i;

  for(i=0; i<g_num_threads; i++) {
    thread_call(world[i].thread, physics_update, (void*) &world[i]);
  }

  for(i=0; i<g_num_threads; i++) {
    thread_wait_call(world[i].thread);
  }

  for(i=0; i<g_num_threads; i++) {
    list_clear(world[i].collision_list);
    thread_call(world[i].thread, physics_detect_collisions, (void*) &world[i]);
  }

  for(i=0; i<g_num_threads; i++) {
    thread_wait_call(world[i].thread);
  }

  for(i=0; i<g_num_threads; i++) {
    thread_call(world[i].thread, physics_handle_collisions, (void*) &world[i]);
  }

  for(i=0; i<g_num_threads; i++) {
    thread_wait_call(world[i].thread);
  }
}

void physics_terminate() {
  int i;

  for(i=0; i<g_num_threads; i++) {
    list_clear(world[i].solid_list);
    list_clear(world[i].collision_list);

    thread_join(world[i].thread);
    fprintf(stderr, "[%s left.]\n", world[i].thread->name);

    free(world[i].thread);
  }

  free(world);
}
