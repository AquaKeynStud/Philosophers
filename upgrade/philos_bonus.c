/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 03:37:12 by arocca            #+#    #+#             */
/*   Updated: 2025/07/24 21:13:33 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	clean_philos(t_monitor *monitor)
{
	unsigned long	i;
	char			*name;

	i = 0;
	while (i < monitor->params->philos_count)
	{
		sem_close(monitor->philos[i].meal_lock);
		name = name_sem("/philo_meal_", monitor->philos[i].id);
		sem_unlink(name);
		free(name);
		i++;
	}
	free(monitor->philos);
	monitor->philos = NULL;
}

static void	*detect_death(void *arg)
{
	t_philo_bonus	*philo;
	t_monitor		*monitor;

	philo = (t_philo_bonus *)arg;
	monitor = philo->monitor;
	while (1)
	{
		sem_wait(philo->meal_lock);
		if (get_time() - philo->last_meal > monitor->params->time_to_die)
		{
			sem_wait(philo->monitor->write);
			printf("%lu %d %s\n", timestamp(monitor->start), philo->id, "died");
			sem_post(philo->meal_lock);
			exit(1);
		}
		sem_post(philo->meal_lock);
		ms_wait(10);
	}
	return (NULL);
}

static void	*stop_detector(void *arg)
{
	t_philo_bonus	*philo;

	philo = (t_philo_bonus *)arg;
	sem_wait(philo->monitor->stop);
	sem_wait(philo->monitor->write);
	exit(0);
}

void	eat_bonus(t_philo_bonus *philo, t_monitor *monitor, int id)
{
	sem_wait(philo->meal_lock);
	philo->last_meal = get_time();
	philo->meals++;
	if (philo->meals == monitor->params->max_meals)
	{
		sem_post(monitor->quota);
		sem_post(monitor->write);
		printf("%lu %d %s\n", timestamp(monitor->start), id, "is eating");
	}
	else
		print_action(philo, "is eating");
	sem_post(philo->meal_lock);
	ms_wait(monitor->params->time_to_eat);
}

int	philo_routine(t_philo_bonus *philo)
{
	pthread_t	death_thread;
	pthread_t	quota_thread;

	pthread_create(&death_thread, NULL, detect_death, philo);
	pthread_create(&quota_thread, NULL, stop_detector, philo);
	pthread_detach(death_thread);
	pthread_detach(quota_thread);
	while (1)
	{
		print_action(philo, "is thinking");
		sem_wait(philo->monitor->forks);
		print_action(philo, "has taken a fork");
		sem_wait(philo->monitor->forks);
		print_action(philo, "has taken a fork");
		eat_bonus(philo, philo->monitor, philo->id);
		sem_post(philo->monitor->forks);
		sem_post(philo->monitor->forks);
		print_action(philo, "is sleeping");
		ms_wait(philo->monitor->params->time_to_sleep);
	}
}
