/**
 * @file uthread.h
 * @brief Cross-platform thread macros.
 */
#pragma once
#ifdef _WIN32
#include <windows.h>

#define ROUTINE_RETURN_TYPE void
#define ROUTINE_RETURN return

typedef HANDLE             thread;
typedef CRITICAL_SECTION   mutex;
typedef CONDITION_VARIABLE cond;

#define create_thread(thread, routine, arg) \
  (thread = CreateThread(0, 0, routine, 0, 0, NULL))

#define kill_thread(thread) \
  (TerminateThread(hthread, 0))

#define wait_thread(thread) \
  (WaitForSingleObject(thread, INFINITE))

#define exit_thread(void) \
  (ExitThread(0))

#define create_mutex(mutex) \
  (InitializeCriticalSection(mutex))

#define destroy_mutex(mutex) \
  (DeleteCriticalSection(mutex))

#define lock_mutex(mutex) \
  (EnterCriticalSection(mutex))

#define try_lock_mutex(mutex) \
  (TryEnterCriticalSection(mutex))

#define unlock_mutex(mutex) \
  (LeaveCriticalSection(mutex))

#define create_cond(cond) \
  (InitializeConditionVariable(cond))

#define destroy_cond(cond) \
  ((void)(cond))

#define signal_cond(cond) \
  (WakeConditionVariable(cond))

#define broadcast_cond(cond) \
  (WakeAllConditionVariable(cond))

#define wait_cond(cond, mutex) \
  (SleepConditionVariableCS(cond, mutex, INFINITE))

#define sleep_for_ms(int ms) \
  (Sleep(ms))

#else
#include <unistd.h>
#include <pthread.h>

#define ROUTINE_RETURN_TYPE void*
#define ROUTINE_RETURN return NULL

typedef pthread_t       thread;
typedef pthread_mutex_t mutex;
typedef pthread_cond_t  cond;

#define create_thread(thread, routine, arg) \
  (pthread_create(thread, 0, routine, arg))

#define kill_thread(thread) \
  (pthread_kill(thread, 0))

#define wait_thread(thread) \
  (pthread_join(thread, NULL))

#define exit_thread(void) \
  (pthread_exit(NULL))

#define create_mutex(mutex) \
  (pthread_mutex_init(mutex, 0))

#define destroy_mutex(mutex) \
  (pthread_mutex_destroy(mutex))

#define lock_mutex(mutex) \
  (pthread_mutex_lock(mutex))

#define try_lock_mutex(mutex) \
  (pthread_mutex_trylock(mutex))

#define unlock_mutex(mutex) \
  (pthread_mutex_unlock(mutex))

#define create_cond(cond) \
  (pthread_cond_init(cond, 0))

#define destroy_cond(cond) \
  (pthread_cond_destroy(cond))

#define signal_cond(cond) \
  (pthread_cond_signal(cond))

#define broadcast_cond(cond) \
  (pthread_cond_broadcast(cond))

#define wait_cond(cond, mutex) \
  (pthread_cond_wait(cond, mutex))

#define sleep_for_ms(ms) \
  (usleep_ms(ms * 1000))
#endif
