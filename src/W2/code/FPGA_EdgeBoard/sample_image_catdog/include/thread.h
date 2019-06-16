#ifndef _THREAD__H_
#define _THREAD__H_


#ifndef _WIN32
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#ifdef _WIN32
#define pthread_mutex_t HANDLE
#define CREATE_MUTEX(mutex) mutex = CreateMutex(NULL, FALSE, NULL)
#define DESTROY_MUTEX(mutex) CloseHandle(mutex)
#define MUTEX_LOCK(mutex) WaitForSingleObject(mutex, INFINITE)
#define MUTEX_UNLOCK(mutex) ReleaseMutex(mutex)
#define pthread_rwlock_t HANDLE
#define CREATE_RWLOCK(mutex) mutex = CreateMutex(NULL, FALSE, NULL)
#define DESTROY_RWLOCK(mutex) CloseHandle(mutex)
#define RWLOCK_RDLOCK(mutex) WaitForSingleObject(mutex, INFINITE)
#define RWLOCK_WRLOCK(mutex) WaitForSingleObject(mutex, INFINITE)
#define RWLOCK_UNLOCK(mutex) ReleaseMutex(mutex)
#define SLEEP_MS(interval) Sleep(interval)
#define pthread_t HANDLE
#define THREAD_RETYPE DWORD WINAPI
#define CREATE_THREAD(thread, proc, param) thread = CreateThread(NULL, 0, proc, param, 0, NULL)
#define DESTROY_THREAD(thread) WaitForSingleObject(thread, INFINITE);CloseHandle(thread)
#else
#define CREATE_MUTEX(mutex) pthread_mutex_init(&mutex, NULL)
#define DESTROY_MUTEX(mutex) pthread_mutex_destroy(&mutex)
#define MUTEX_LOCK(mutex) pthread_mutex_lock(&mutex)
#define MUTEX_UNLOCK(mutex) pthread_mutex_unlock(&mutex)
#define CREATE_RWLOCK(rwlock) pthread_rwlock_init(&rwlock, NULL)
#define DESTROY_RWLOCK(rwlock) pthread_rwlock_destroy(&rwlock)
#define RWLOCK_RDLOCK(rwlock) pthread_rwlock_rdlock(&rwlock)
#define RWLOCK_WRLOCK(rwlock) pthread_rwlock_wrlock(&rwlock)
#define RWLOCK_UNLOCK(rwlock) pthread_rwlock_unlock(&rwlock)
#define SLEEP_MS(interval) usleep(interval * 1000)
#define THREAD_RETYPE void*
#define CREATE_THREAD(thread, proc, param) pthread_create(&thread, NULL, proc, param)
#define DESTROY_THREAD(thread) pthread_join(thread, NULL);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif