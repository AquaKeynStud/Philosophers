/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 03:37:12 by arocca            #+#    #+#             */
/*   Updated: 2025/09/11 10:39:41 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	*detect_death(void *arg)
{
	t_philo_bonus	*philo;
	t_monitor		*monitor;

	philo = (t_philo_bonus *)arg;
	monitor = philo->monitor;
	while (1)
	{
		if (!philo->active)
			return (NULL);
		sem_wait(philo->meal_lock);
		if (get_time() - philo->last_meal >= monitor->params->time_to_die)
		{
			sem_wait(philo->monitor->write);
			printf("\e[1;34m-{%lu}-", timestamp(monitor->start));
			printf("\e[0;1m %d died 🩻\e[0m\n", philo->id);
			sem_post(philo->meal_lock);
			philo->active = false;
			clean_exit(monitor, 1);
		}
		sem_post(philo->meal_lock);
		ms_wait(1);
	}
	return (NULL);
}

static void	*stop_detector(void *arg)
{
	t_philo_bonus	*philo;

	philo = (t_philo_bonus *)arg;
	sem_wait(philo->monitor->stop);
	if (!philo->active)
		return (NULL);
	philo->active = false;
	clean_exit(philo->monitor, 1);
	return (NULL);
}

static void	eat_bonus(t_philo_bonus *philo, t_monitor *monitor, int id)
{
	if (!philo->active)
		return ;
	sem_wait(philo->meal_lock);
	philo->last_meal = get_time();
	philo->meals++;
	if (philo->meals == monitor->params->max_meals)
	{
		sem_post(monitor->quota);
		sem_wait(monitor->write);
		printf("\e[1;34m-{%lu}-", timestamp(monitor->start));
		printf("\e[0;1m %d is eating 🍜\e[0m\n", id);
	}
	else
		print_action(philo, "is eating 🍜");
	ms_wait(monitor->params->time_to_eat);
	sem_post(philo->meal_lock);
}

static void	take_forks_bonus(t_philo_bonus *philo)
{
	sem_wait(philo->monitor->limit);
	sem_wait(philo->monitor->forks);
	print_action(philo, "has taken a fork 🍴");
	sem_wait(philo->monitor->forks);
	print_action(philo, "has taken a fork 🍴");
}

int	philo_routine(t_philo_bonus *philo)
{
	pthread_t	death_thread;
	pthread_t	quota_thread;

	while (get_time() < philo->monitor->start)
		usleep(100);
	pthread_create(&death_thread, NULL, detect_death, philo);
	pthread_create(&quota_thread, NULL, stop_detector, philo);
	pthread_detach(death_thread);
	pthread_detach(quota_thread);
	while (1)
	{
		print_action(philo, "is thinking 💭");
		if (philo->monitor->params->nb_philo % 2)
			usleep(500);
		take_forks_bonus(philo);
		eat_bonus(philo, philo->monitor, philo->id);
		sem_post(philo->monitor->forks);
		sem_post(philo->monitor->forks);
		sem_post(philo->monitor->limit);
		print_action(philo, "is sleeping 💤");
		ms_wait(philo->monitor->params->time_to_sleep);
	}
}
