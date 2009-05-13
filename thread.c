#include <GL/glfw.h>
#include <stdio.h>
#include <stdlib.h>

#include "thread.h"

void GLFWCALL thread_loop(void* arg);


Thread* thread_create(char* name) {
  Thread* thread = malloc(sizeof(Thread));

  sprintf(thread->name, "%s", name);
  thread->alive = 0;
  thread->param = NULL;
  thread->function = NULL;
  thread->signal = glfwCreateCond();
  thread->lock = glfwCreateMutex();
  thread->thread_id = glfwCreateThread(thread_loop, (void*) thread);

  glfwLockMutex(thread->lock);

  while(0 == thread->alive) {
    glfwWaitCond(thread->signal, thread->lock, GLFW_INFINITY);
  }

  glfwUnlockMutex(thread->lock);

  return thread;
}

void thread_signal(Thread* thread) {
  glfwSignalCond(thread->signal);
}

void thread_lock(Thread* thread) {
  glfwLockMutex(thread->lock);
}

void thread_release(Thread* thread) {
  glfwUnlockMutex(thread->lock);
}

void thread_wait_call(Thread* thread) {
  //fprintf(stderr, "[WAIT: %s", thread->name);
  thread_lock(thread);
  
  while(1 == thread->awake) {
    thread_signal(thread);
    glfwWaitCond(thread->signal, thread->lock, GLFW_INFINITY);
  }
  
  thread_release(thread);
  //fprintf(stderr, "]\n");
}

void thread_call(Thread* thread, void (*function)(void*), void* param) {
  //fprintf(stderr, "[CALL: %s", thread->name);
  thread_lock(thread);
  thread->function = function;
  thread->param = param;
  thread->awake = 1;
  thread_release(thread);
  thread_signal(thread);
  //fprintf(stderr, "]\n");
}

void thread_join(Thread* thread) {
  thread_lock(thread);
  thread->alive = 0;
  thread->awake = 1;
  thread_signal(thread);

  glfwWaitCond(thread->signal, thread->lock, GLFW_INFINITY);
  glfwWaitThread(thread->thread_id, GLFW_INFINITY);
  thread_release(thread);
  glfwDestroyMutex(thread->lock);
  glfwDestroyCond(thread->signal);
}

void GLFWCALL thread_loop(void* arg) {
  Thread* thread = (Thread*) arg;


  glfwLockMutex(thread->lock);

  thread->alive = 1;
  thread->awake = 0;

  //fprintf(stderr, "%s came alive!\n", thread->name);

  glfwSignalCond(thread->signal);

  while(1 == thread->alive) {    
    while(0 == thread->awake) {
      thread_signal(thread);
      glfwWaitCond(thread->signal, thread->lock, GLFW_INFINITY);
    }

    if(NULL != thread->function && 1 == thread->alive) {
      thread->function(thread->param);
    }

    thread->awake = 0;
    thread_signal(thread);
  }

  thread_signal(thread);
  glfwUnlockMutex(thread->lock);
}
