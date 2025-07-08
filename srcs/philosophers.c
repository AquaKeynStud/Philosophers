/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 16:06:30 by arocca            #+#    #+#             */
/*   Updated: 2025/07/08 15:43:22 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	take_forks(t_data *data, t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print(data, philo, timestamp(data), "has taken a fork");
		if (stopped(data))
		{
			pthread_mutex_unlock(philo->right_fork);
			return (false);
		}
		pthread_mutex_lock(philo->left_fork);
		print(data, philo, timestamp(data), "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print(data, philo, timestamp(data), "has taken a fork");
		if (stopped(data))
		{
			pthread_mutex_unlock(philo->left_fork);
			return (false);
		}
		pthread_mutex_lock(philo->right_fork);
		print(data, philo, timestamp(data), "has taken a fork");
	}

	// Re-vérifie l’état après avoir les deux
	if (stopped(data))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (false);
	}
	return (true);
}

static void	eat(t_data *data, t_philo *philo)
{
	if (!take_forks(data, philo))
		return ;

	philo->is_eating = true;
	ms_wait(data->params.time_to_eat);
	print(data, philo, timestamp(data), "is eating");
	
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = get_time();
	philo->meals++;
	pthread_mutex_unlock(&philo->meal_mutex);
	
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	philo->is_eating = false;
}

void	*routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->monitoring;
	while (get_time() < data->start)
		usleep(100);
	while (!stopped(data))
	{
		print(data, philo, timestamp(data), "is thinking");

		if (stopped(data))
			break ;

		eat(data, philo);

		if (stopped(data))
			break ;

		print(data, philo, timestamp(data), "is sleeping");
		ms_wait(data->params.time_to_sleep);
	}
	return (NULL);
}
