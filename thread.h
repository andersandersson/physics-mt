#ifndef THREAD_H
#define THREAD_H

typedef struct Thread {
  char name[1024];
  void (*function)(void*);
  void* param;
  GLFWcond signal;
  GLFWmutex lock;
  GLFWthread thread_id;
  int alive;
  int awake;
} Thread;

Thread* thread_create(char* name);
void thread_signal(Thread* thread);
void thread_lock(Thread* thread);
void thread_release(Thread* thread);
void thread_wait_call(Thread* thread);
void thread_call(Thread* thread, void (*function)(void*), void* param);
void thread_join(Thread* thread);

#endif // THREAD_H
