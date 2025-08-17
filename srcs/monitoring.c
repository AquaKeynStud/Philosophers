/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 16:06:04 by arocca            #+#    #+#             */
/*   Updated: 2025/07/22 20:46:52 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	stopped(t_data *data)
{
	bool	stopped;

	pthread_mutex_lock(&data->state);
	stopped = data->stop;
	pthread_mutex_unlock(&data->state);
	return (stopped);
}

static bool	is_dead(t_data *data, t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	if (get_time() - philo->last_meal >= data->params.time_to_die)
	{
		print(data, philo, "died");
		pthread_mutex_unlock(&philo->meal_mutex);
		pthread_mutex_lock(&data->state);
		data->stop = true;
		pthread_mutex_unlock(&data->state);
		return (true);
	}
	pthread_mutex_unlock(&philo->meal_mutex);
	return (false);
}

static void	*philo_died(void *arg)
{
	unsigned long	i;
	t_data			*data;

	data = (t_data *)arg;
	while (!stopped(data))
	{
		i = 0;
		while (i < data->params.nb_philo && !stopped(data))
		{
			if (is_dead(data, &data->philos[i++]))
				return (NULL);
		}
		ms_wait(50);
	}
	return (NULL);
}

static void	*check_quota(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	if (data->params.max_meals <= 0)
		return (NULL);
	while (!stopped(data))
	{
		pthread_mutex_lock(&data->state);
		if (data->quota == data->params.nb_philo)
		{
			data->stop = true;
			pthread_mutex_unlock(&data->state);
			return (NULL);
		}
		pthread_mutex_unlock(&data->state);
		ms_wait(50);
	}
	return (NULL);
}

void	*monitoring(void *arg)
{
	t_data		*data;
	pthread_t	thread_death;
	pthread_t	thread_quota;

	data = (t_data *)arg;
	while (get_time() < data->start)
		usleep(100);
	pthread_create(&thread_death, NULL, philo_died, data);
	pthread_create(&thread_quota, NULL, check_quota, data);
	pthread_join(thread_death, NULL);
	pthread_join(thread_quota, NULL);
	return (NULL);
}
