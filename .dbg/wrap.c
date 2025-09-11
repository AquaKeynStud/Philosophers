#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>

#ifndef MTXLIM
	#define MTXLIM 1
#endif

#ifndef SEMLIM
	#define SEMLIM 1
#endif

static void		*(*real_malloc)(size_t size) = NULL;
static sem_t	*(*real_sem_open)(const char *name, int oflag, ...) = NULL;
static int		(*real_pthread_mutex_init)(pthread_mutex_t *, const pthread_mutexattr_t *) = NULL;

int	pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
	static int count = 0;
	count++;

	if (!real_pthread_mutex_init)
		real_pthread_mutex_init = dlsym(RTLD_NEXT, "pthread_mutex_init");
	if (count < MTXLIM)
		return (real_pthread_mutex_init(mutex, attr));
	else
		return (1);
}

sem_t *sem_open(const char *name, int oflag, ...)
{
	va_list ap;
	mode_t mode = 0;
	static int count = 0;
	unsigned int value = 0;
	
	count++;
	if (!real_sem_open)
		real_sem_open = dlsym(RTLD_NEXT, "sem_open");
	if (oflag & O_CREAT)
	{
		va_start(ap, oflag);
		mode = va_arg(ap, mode_t);
		value = va_arg(ap, unsigned int);
		va_end(ap);
	}
	if (count < SEMLIM)
	{
		if (oflag & O_CREAT)
			return real_sem_open(name, oflag, mode, value);
		else
			return real_sem_open(name, oflag);
	}
	else
		return (SEM_FAILED);
}
