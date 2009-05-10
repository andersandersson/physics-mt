#include <GL/glfw.h>
#include <stdio.h>
#include <stdlib.h>

#include "physics.h"
#include "list.h"

typedef struct Thread {
  char* name;
  void (*function)(void* param);
  void* param;
  GLFWcond signal;
  GLFWmutex lock;
  GLFWthread thread_id;
  int alive;
} Thread;

void GLFWCALL thread_loop(void* arg);


Thread* thread_create(char* name) {
  Thread* thread = malloc(sizeof(Thread));

  thread->name = name;
  thread->alive = 0;
  thread->param = NULL;
  thread->function = NULL;
  thread->signal = glfwCreateCond();
  thread->lock = glfwCreateMutex();
  thread->thread_id = glfwCreateThread(thread_loop, (void*) thread);

  return thread;
}

void thread_signal(Thread* thread) {
  glfwSignalCond(thread->signal);
}

void thread_join(Thread* thread) {
  glfwLockMutex(thread->lock);
  thread->alive = 0;
  glfwUnlockMutex(thread->lock);

  thread_signal(thread);

  glfwWaitThread(thread->thread_id, GLFW_INFINITY);
  glfwDestroyMutex(thread->lock);
  glfwDestroyCond(thread->signal);
}

void GLFWCALL thread_loop(void* arg) {
  Thread* thread = (Thread*) arg;

  thread->alive = 1;

  while(1 == thread->alive) {
    glfwLockMutex(thread->lock);

    if(NULL != thread->function) {
      thread->function(thread->param);
    }

    glfwWaitCond(thread->signal, thread->lock, GLFW_INFINITY);
  }
}


int g_num_threads = 1;
List** g_world_parts;
Thread** g_world_threads;


void physics_init(int threads) {
  int i;
  g_num_threads = threads;

  g_world_parts = malloc(g_num_threads*sizeof(List*));
  g_world_threads = malloc(g_num_threads*sizeof(Thread*));

  for(i=0; i<g_num_threads; i++) {
    g_world_parts[i] = malloc(sizeof(List));
    g_world_threads[i] = thread_create();
  }
}

void physics_tick() {
  
}

void physics_terminate() {
  int i;

  for(i=0; i<g_num_threads; i++) {
    free(g_world_parts[i]);

    thread_join(g_world_threads[i]);

    free(g_world_threads[i]);
  }
  
  free(g_world_threads);
  free(g_world_parts);
}
