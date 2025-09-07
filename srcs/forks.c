/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 09:57:52 by arocca            #+#    #+#             */
/*   Updated: 2025/09/07 10:15:26 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	mutex_lock(t_fork *fork)
{
	pthread_mutex_lock(&fork->fork);
	fork->isTaken = true;
	return ;
}

void	mutex_unlock(t_fork *fork)
{
	fork->isTaken = false;
	pthread_mutex_unlock(&fork->fork);
	return ;
}

void	clean_forks(t_data *data, unsigned long failed)
{
	unsigned long	i;
	unsigned long	stop;

	i = 0;
	if (failed)
		stop = failed;
	else
		stop = data->params.nb_philo;
	while (i < stop)
		pthread_mutex_destroy(&data->forks[i++].fork);
	free(data->forks);
	return ;
}

void	clean_meals(t_data *data, unsigned long failed)
{
	unsigned long	i;
	unsigned long	stop;

	i = 0;
	if (failed)
		stop = failed;
	else
		stop = data->params.nb_philo;
	clean_forks(data, 0);
	while (i < stop)
		pthread_mutex_destroy(&data->philos[i++].meal_mutex);
	free(data->philos);
	exit_err("Meal mutex init failed");
	return ;
}