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

static bool	is_dead(t_data *data, t_philo *philo, unsigned long ttk)
{
	pthread_mutex_lock(&philo->meal_mutex);
	if (get_time() - philo->last_meal >= ttk && !philo->is_eating)
	{
		pthread_mutex_unlock(&philo->meal_mutex);
		print(data, philo, "died");
		data->can = true;
		return (true);
	}
	pthread_mutex_unlock(&philo->meal_mutex);
	return (false);
}

bool	philo_died(t_data *data)
{
	unsigned long	i;

	i = 0;
	while (i < data->params.philos_count)
	{
		if (is_dead(data, &data->philos[i], data->params.time_to_die))
			return (true);
		i++;
	}
	return (false);
}

static bool	everyone_ate_enough(t_data *data)
{
	unsigned long	i;
	t_philo			*philos;

	philos = data->philos;
	if (data->params.max_meals <= 0)
		return (false);
	i = 0;
	while (i < data->params.philos_count)
	{
		pthread_mutex_lock(&philos[i].meal_mutex);
		if (philos[i].meals < data->params.max_meals)
		{
			pthread_mutex_unlock(&philos[i].meal_mutex);
			return (false);
		}
		pthread_mutex_unlock(&philos[i].meal_mutex);
		i++;
	}
	return (true);
}

void	*monitoring(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		if (philo_died(data))
			break ;
		else if (everyone_ate_enough(data))
			break ;
		ms_wait(0.9);
	}
	pthread_mutex_lock(&data->state);
	data->stop = true;
	pthread_mutex_unlock(&data->state);
	return (NULL);
}
