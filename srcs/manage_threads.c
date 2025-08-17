/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_threads.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 08:25:35 by arocca            #+#    #+#             */
/*   Updated: 2025/08/17 10:34:31 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_all(t_data *data, pthread_t monitor)
{
	unsigned long	i;

	i = 0;
	pthread_join(monitor, NULL);
	while (i < data->params.nb_philo)
		pthread_join(data->philos[i++].thread, NULL);
}

void	cleanup(t_data *data)
{
	unsigned long	i;

	i = 0;
	while (i < data->params.nb_philo)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philos[i++].meal_mutex);
	}
	free(data->forks);
	pthread_mutex_destroy(&data->state);
	pthread_mutex_destroy(&data->printer);
	free(data->philos);
}

int	start_philosophers(t_data *data)
{
	unsigned long	i;
	t_philo			*philo;

	i = 0;
	philo = data->philos;
	while (i < data->params.nb_philo)
	{
		if (pthread_create(&philo[i].thread, NULL, routine, &philo[i]))
			return (true);
		i++;
	}
	return (false);
}

int	start_monitor(t_data *data, pthread_t *monitor_thread)
{
	if (pthread_create(monitor_thread, NULL, monitoring, data))
		return (true);
	return (false);
}
