/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 16:06:30 by arocca            #+#    #+#             */
/*   Updated: 2025/09/09 11:47:58 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	take_forks_odd(t_data *data, t_philo *philo)
{
	t_fork	*first;
	t_fork	*second;

	while (1)
	{
		first = philo->left_fork;
		second = philo->right_fork;
		if (mutex_trylock(first))
			break ;
		first = philo->right_fork;
		second = philo->left_fork;
		if (mutex_trylock(first))
			break ;
		ms_wait(1);
	}
	print(data, philo, "has taken a fork 🍴");
	if (release(data, first, NULL))
		return (false);
	while (!mutex_trylock(second))
		ms_wait(1);
	print(data, philo, "has taken a fork 🍴");
	if (release(data, second, first))
		return (false);
	return (true);
}

static bool	take_forks_even(t_data *data, t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		mutex_lock(philo->right_fork);
		print(data, philo, "has taken a fork 🍴");
		if (release(data, philo->right_fork, NULL))
			return (false);
		mutex_lock(philo->left_fork);
		print(data, philo, "has taken a fork 🍴");
	}
	else
	{
		mutex_lock(philo->left_fork);
		print(data, philo, "has taken a fork 🍴");
		if (release(data, philo->left_fork, NULL))
			return (false);
		mutex_lock(philo->right_fork);
		print(data, philo, "has taken a fork 🍴");
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
			printf("\e[96m\e[1m-{%ld}-\e[0m", timestamp(data->start));
			printf("\e[1m %d is eating 🍝\e[0m\n", philo->id);
			pthread_mutex_lock(&data->state);
			data->stop = true;
			pthread_mutex_unlock(&data->state);
			pthread_mutex_unlock(&data->printer);
			return (true);
		}
	}
	print(data, philo, "is eating 🍝");
	return (false);
}

static bool	eat(t_data *data, t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = get_time();
	philo->meals++;
	if (is_last_for_quota(data, philo))
	{
		mutex_unlock(philo->left_fork);
		mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(&philo->meal_mutex);
		return (false);
	}
	pthread_mutex_unlock(&philo->meal_mutex);
	ms_wait(data->params.time_to_eat);
	mutex_unlock(philo->left_fork);
	mutex_unlock(philo->right_fork);
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
		print(data, philo, "is thinking 💭");
		if ((data->params.nb_philo % 2) && !take_forks_odd(data, philo))
			return (NULL);
		else if (!(data->params.nb_philo % 2) && !take_forks_even(data, philo))
			return (NULL);
		if (stopped(data) || !eat(data, philo))
			break ;
		if (stopped(data))
			break ;
		print(data, philo, "is sleeping 💤");
		ms_wait(data->params.time_to_sleep);
	}
	return (NULL);
}
