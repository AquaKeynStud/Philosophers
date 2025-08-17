/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 16:06:30 by arocca            #+#    #+#             */
/*   Updated: 2025/08/02 14:46:35 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	release(t_data *data, pthread_mutex_t *one, pthread_mutex_t *two)
{
	if (stopped(data))
	{
		if (one)
			pthread_mutex_unlock(one);
		if (two)
			pthread_mutex_unlock(two);
		return (true);
	}
	return (false);
}

static bool	take_forks(t_data *data, t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print(data, philo, "has taken a fork");
		if (release(data, philo->right_fork, NULL))
			return (false);
		pthread_mutex_lock(philo->left_fork);
		print(data, philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print(data, philo, "has taken a fork");
		if (release(data, philo->left_fork, NULL))
			return (false);
		pthread_mutex_lock(philo->right_fork);
		print(data, philo, "has taken a fork");
	}
	if (release(data, philo->left_fork, philo->right_fork))
		return (false);
	return (true);
}

static bool	is_last_for_quota(t_data *data, t_philo *philo)
{
	bool	last;
	t_data	*monitor;

	monitor = philo->monitoring;
	if (philo->meals == monitor->params.max_meals)
	{
		pthread_mutex_lock(&data->state);
		monitor->quota++;
		last = monitor->quota == monitor->params.nb_philo;
		pthread_mutex_unlock(&data->state);
		if (last)
		{
			pthread_mutex_lock(&data->printer);
			printf("%ld %d is eating\n", timestamp(data->start), philo->id);
			pthread_mutex_unlock(&data->printer);
			return (true);
		}
		else
			print(data, philo, "is eating");
	}
	else
		print(data, philo, "is eating");
	return (false);
}

static bool	eat(t_data *data, t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = get_time();
	philo->meals++;
	if (is_last_for_quota(data, philo))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(&philo->meal_mutex);
		return (false);
	}
	pthread_mutex_unlock(&philo->meal_mutex);
	ms_wait(data->params.time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (true);
}

void	*routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->monitoring;
	while (get_time() < data->start)
		usleep(100);
	while (1)
	{
		print(data, philo, "is thinking");
		if (!take_forks(data, philo))
			return (NULL);
		if (stopped(data) || !eat(data, philo))
			break ;
		if (stopped(data))
			break ;
		print(data, philo, "is sleeping");
		ms_wait(data->params.time_to_sleep);
	}
	return (NULL);
}
