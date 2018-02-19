#pragma once

#ifdef __linux__
#include <pthread.h>
#define SECT pthread_mutex_t
#define lock_init(x) pthread_mutex_init(x, NULL)
#define lock_free(x) pthread_mutex_destroy(x)
#define lock_lock(x) pthread_mutex_lock(x)
#define lock_unlock(x) pthread_mutex_unlock(x)
#else
#include <windows.h>
#define SECT CRITICAL_SECTION
#define lock_init(x) InitializeCriticalSection(x)
#define lock_free(x) DeleteCriticalSection(x)
#define lock_lock(x) EnterCriticalSection(x)
#define lock_unlock(x) LeaveCriticalSection(x)
#endif
#define lock_new(x) LockNew(x)
#define LSECT SECT*

inline LSECT LockNew() {
	LSECT s = (LSECT)malloc(sizeof(SECT));
	lock_init(s);
	return s;
}