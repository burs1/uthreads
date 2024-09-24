#pragma once
#include <sys/_pthread/_pthread_mutex_t.h>
#ifdef _WIN32
#include <windows.h>

#define UT_ROUTINE_RETURN_TYPE void

typedef HANDLE             ut_thread_t;
typedef CRITICAL_SECTION   ut_mutex_t;
typedef CONDITION_VARIABLE ut_cond_t;

typedef UUT_ROUTINE_RETURN_TYPE(*ut_routine_t)(void *arg);

inline int ut_thread_create(
  ut_thread_t *thread, ut_routine_t routine, void *arg
) {
  DWORD thread_id;
  return (*thread = CreateThread(0, 0, routine, 0, 0, &thread_id));
}

inline int ut_thread_kill(ut_thread_t thread) {
  return TerminateThread(hthread, 0);
}

inline int ut_thread_wait(ut_thread_t thread) {
  return WaitForSingleObject(thread, INFINITE);
}

inline void ut_thread_exit(void) {
  ExitThread(0);
}

inline int ut_mutex_create(ut_mutex_t *mutex) {
  InitializeCriticalSection(mutex);
}

inline int ut_mutex_destroy(ut_mutex_t *mutex) {
  DeleteCriticalSection(mutex);
  return 1;
}

inline int ut_mutex_lock(ut_mutex_t *mutex) {
  EnterCriticalSection(mutex);
  return 1;
}

inline int ut_mutex_trylock(ut_mutex_t *mutex) {
  #warning "ut_mutex_trylock() not implemented"
}

inline int ut_mutex_unlock(ut_mutex_t *mutex) {
  LeaveCriticalSection(&mutex->critical_section);
  return 1;
}

inline int ut_cond_create(ut_cond_t *cond) {
  InitializeConditionVariable(cond);
  return 1;
}

inline int ut_cond_destroy(ut_cond_t *cond) {
  (void)(cond);
}

inline int ut_cond_signal(ut_cond_t *cond) {
  WakeConditionVariable(cond);
  return 1;
}

inline int ut_cond_broadcast(ut_cond_t *cond) {
  WakeAllConditionVariable(cond);
  return 1;
}

inline int ut_cond_wait(ut_cond_t *cond, ut_mutex_t *mutex) {
  SleepConditionVariableCS(cond, mutex, INFINITE);
  return 1;
}

inline int ut_sleep(int ms) {
}
#else
#include <unistd.h>
#include <pthread.h>

#define UT_ROUTINE_RETURN_TYPE void*

typedef pthread_t       ut_thread_t;
typedef pthread_mutex_t ut_mutex_t;
typedef pthread_cond_t  ut_cond_t;

typedef UT_ROUTINE_RETURN_TYPE(*ut_routine_t)(void *arg);

inline int ut_thread_create(
  ut_thread_t *thread, ut_routine_t routine, void *arg
) {
  return pthread_create(thread, 0, routine, arg) == 0;
}

inline int ut_thread_kill(ut_thread_t thread) {
  return pthread_kill(thread, 0) == 0;
}

inline int ut_thread_wait(ut_thread_t thread) {
  return pthread_join(thread, NULL) == 0;
}

inline void ut_thread_exit(void) {
  pthread_exit(NULL);
}

inline int ut_mutex_create(ut_mutex_t *mutex) {
  return pthread_mutex_init(mutex, 0) == 0;
}

inline int ut_mutex_destroy(ut_mutex_t *mutex) {
  return pthread_mutex_destroy(mutex) == 0;
}

inline int ut_mutex_lock(ut_mutex_t *mutex) {
  return pthread_mutex_lock(mutex) == 0;
}

inline int ut_mutex_trylock(ut_mutex_t *mutex) {
  return pthread_mutex_trylock(mutex) == 0;
}

inline int ut_mutex_unlock(ut_mutex_t *mutex) {
  return pthread_mutex_unlock(mutex) == 0;
}

inline int ut_cond_create(ut_cond_t *cond) {
  return pthread_cond_init(cond, 0) == 0;
}

inline int ut_cond_destroy(ut_cond_t *cond) {
  return pthread_cond_destroy(cond) == 0;
}

inline int ut_cond_signal(ut_cond_t *cond) {
  return pthread_cond_signal(cond) == 0;
}

inline int ut_cond_broadcast(ut_cond_t *cond) {
  return pthread_cond_broadcast(cond) == 0;
}

inline int ut_cond_wait(ut_cond_t *cond, ut_mutex_t *mutex) {
  return pthread_cond_wait(cond, mutex);
}

inline int ut_sleep(int ms) {
  return usleep(ms * 1000) == 0;
}
#endif

