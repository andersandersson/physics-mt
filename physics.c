#include <GL/glfw.h>
#include <stdio.h>
#include <stdlib.h>

#include "physics.h"
#include "thread.h"
#include "list.h"

int g_counter = 0;
int g_num_threads = 1;
List** g_world_parts;
Thread** g_world_threads;


void physics_init(int threads) {
  int i;
  char name[1024];
  g_num_threads = threads;

  g_world_parts = malloc(g_num_threads*sizeof(List*));
  g_world_threads = malloc(g_num_threads*sizeof(Thread*));

  for(i=0; i<g_num_threads; i++) {
    sprintf(name, "[Thread %d]", i);
    g_world_parts[i] = malloc(sizeof(List));
    g_world_threads[i] = thread_create(name);
  }
}

void physics_part_tick_1(void* arg) {
  Thread* thread = (Thread*) arg;
  fprintf(stderr, "[%d] %s: Updating physics\n", g_counter, thread->name);
}

void physics_part_tick_2(void* arg) {
  Thread* thread = (Thread*) arg;
  fprintf(stderr, "[%d] %s: Detecting collisions\n", g_counter, thread->name);
}

void physics_tick() {
  int i;
  g_counter++;

  for(i=0; i<g_num_threads; i++) {
    thread_call(g_world_threads[i], physics_part_tick_1, (void*) g_world_threads[i]);
  }

  for(i=0; i<g_num_threads; i++) {
    thread_wait_call(g_world_threads[i]);
  }

  for(i=0; i<g_num_threads; i++) {
    thread_call(g_world_threads[i], physics_part_tick_2, (void*) g_world_threads[i]);
  }

  for(i=0; i<g_num_threads; i++) {
    thread_wait_call(g_world_threads[i]);
  }
}

void physics_terminate() {
  int i;

  for(i=0; i<g_num_threads; i++) {
    free(g_world_parts[i]);

    thread_join(g_world_threads[i]);
    fprintf(stderr, "[%s left.]\n", g_world_threads[i]->name);

    free(g_world_threads[i]);
  }
  
  free(g_world_threads);
  free(g_world_parts);
}
