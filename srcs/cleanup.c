/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:52:39 by arocca            #+#    #+#             */
/*   Updated: 2025/07/08 16:44:36 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_all(t_data *data, pthread_t monitor)
{
	int	i;

	i = 0;
	pthread_join(monitor, NULL);
	while (i < data->params.philos_count)
		pthread_join(data->philos[i++].thread, NULL);
}

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->params.philos_count)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philos[i++].meal_mutex);
	}
	free(data->forks);
	pthread_mutex_destroy(&data->state);
	pthread_mutex_destroy(&data->printer);
	free(data->philos);
}
