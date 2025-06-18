/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 08:25:35 by arocca            #+#    #+#             */
/*   Updated: 2025/06/18 21:43:39 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// monitoring.c
void	*monitoring(void *arg)
{
	int		i;
	long	now, last_meal;
	t_data	*data = arg;

	while (!stopped(data))
	{
		for (i = 0; i < data->params.philos_count; i++)
		{
			now = get_time();
			pthread_mutex_lock(&data->philos[i].meal_mutex);
			last_meal = data->philos[i].last_meal;
			pthread_mutex_unlock(&data->philos[i].meal_mutex);

			if ((now - last_meal) > data->params.time_to_die)
			{
				// Arrêt global
				pthread_mutex_lock(&data->state);
				data->stop = true;
				pthread_mutex_unlock(&data->state);

				// Affichage d’un seul "died"
				pthread_mutex_lock(&data->printer);
				printf("%ld %d died\n", timestamp(data), data->philos[i].id);
				pthread_mutex_unlock(&data->printer);

				return (NULL);
			}
		}
		usleep(500);
	}
	return (NULL);
}


void	*routine(void *arg)
{
	t_philo	*philo = (t_philo *)arg;
	t_data	*data = philo->monitoring;

	while (get_time() < data->start)
		usleep(100);
	while (!stopped(data))
	{
		print(data, philo, timestamp(data), "is thinking");

		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(philo->right_fork);
			print(data, philo, timestamp(data), "has taken a fork");
			pthread_mutex_lock(philo->left_fork);
			print(data, philo, timestamp(data), "has taken a fork");
		}
		else
		{
			pthread_mutex_lock(philo->left_fork);
			print(data, philo, timestamp(data), "has taken a fork");
			pthread_mutex_lock(philo->right_fork);
			print(data, philo, timestamp(data), "has taken a fork");
		}

		// MANGER
		pthread_mutex_lock(&philo->meal_mutex);
		philo->last_meal = get_time();
		philo->meals++;
		pthread_mutex_unlock(&philo->meal_mutex);

		print(data, philo, timestamp(data), "is eating");
		ms_wait(data->params.time_to_eat);

		// Relâcher les fourchettes
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);

		// Vérification après avoir mangé
		if (stopped(data))
			break;

		// DORMIR
		print(data, philo, timestamp(data), "is sleeping");
		ms_wait(data->params.time_to_sleep);
	}
	return (NULL);
}

int start_philosophers(t_data *data)
{
	int i;

	i = 0;
	while (i < data->params.philos_count)
	{
		if (pthread_create(&data->philos[i].thread, NULL, routine, &data->philos[i]))
			return (true);
		i++;
	}
	return (false);
}

int start_monitor(t_data *data, pthread_t *monitor_thread)
{
	if (pthread_create(monitor_thread, NULL, monitoring, data))
		return (true);
	return (false);
}
