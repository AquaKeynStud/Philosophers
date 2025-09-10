#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <dlfcn.h>

#ifndef LIMIT
	#define LIMIT = 4
#endif

static int (*real_pthread_mutex_init)(pthread_mutex_t *, const pthread_mutexattr_t *) = NULL;

int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
	static int count = 0;
	count++;

	if (!real_pthread_mutex_init)
		real_pthread_mutex_init = dlsym(RTLD_NEXT, "pthread_mutex_init");
	if (count < 12)
		return (real_pthread_mutex_init(mutex, attr));
	else
		return (1);
}
