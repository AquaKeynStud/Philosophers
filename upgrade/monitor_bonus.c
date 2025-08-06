/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:13:06 by arocca            #+#    #+#             */
/*   Updated: 2025/07/24 21:13:26 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <sys/wait.h>
#include "philo_bonus.h"

void	clean_exit(t_monitor *monitor, int code)
{
	if (monitor->philos)
		clean_philos(monitor);
	if (monitor->params)
	{
		free(monitor->params);
		monitor->params = NULL;
	}
	if (monitor->pids)
	{
		free(monitor->pids);
		monitor->pids = NULL;
	}
	if (monitor->forks)
		clean_sem(monitor->forks, "/philo_forks");
	if (monitor->write)
		clean_sem(monitor->write, "/philo_write");
	if (monitor->quota)
		clean_sem(monitor->quota, "/philo_quota");
	if (monitor->stop)
		clean_sem(monitor->stop, "/philo_stop");
	exit(code);
}

static void	*get_death(void *arg)
{
	unsigned long	i;
	pid_t			pid;
	t_monitor		*monitor;

	monitor = (t_monitor *)arg;
	pid = waitpid(-1, NULL, 0);
	if (pid != -1)
	{
		i = 0;
		sem_post(monitor->stop);
		while (i < monitor->params->philos_count)
		{
			sem_post(monitor->quota);
			i++;
		}
	}
	return (NULL);
}

static void	*check_quota(void *arg)
{
	unsigned long	i;
	t_monitor		*monitor;

	i = 0;
	monitor = (t_monitor *)arg;
	while (i < monitor->params->philos_count - 1)
	{
		sem_wait(monitor->quota);
		sem_post(monitor->write);
		i++;
	}
	sem_wait(monitor->quota);
	sem_post(monitor->stop);
	return (NULL);
}

void	monitoring_bonus(t_monitor *monitor)
{
	pthread_t	death_thread;
	pthread_t	quota_thread;

	while (get_time() < monitor->start)
		usleep(100);
	pthread_create(&death_thread, NULL, get_death, monitor);
	pthread_create(&quota_thread, NULL, check_quota, monitor);
	pthread_join(death_thread, NULL);
	pthread_join(quota_thread, NULL);
	clean_exit(monitor, 0);
}
