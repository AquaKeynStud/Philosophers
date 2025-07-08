/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 16:06:04 by arocca            #+#    #+#             */
/*   Updated: 2025/07/08 15:08:40 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	is_dead(t_philo *philo, int ttk)
{
	pthread_mutex_lock(&philo->meal_mutex);
	if (get_time() - philo->last_meal >= ttk && !philo->is_eating)
	{
		pthread_mutex_unlock(&philo->meal_mutex);
		return (true);
	}
	pthread_mutex_unlock(&philo->meal_mutex);
	return (false);
}

bool	philo_died(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->params.philos_count)
	{
		if (is_dead(&data->philos[i], data->params.time_to_die))
		{
			print(data, &data->philos[i], timestamp(data), "died");
			pthread_mutex_lock(&data->state);
			data->stop = true;
			pthread_mutex_unlock(&data->state);
			return (true);
		}
		i++;
	}
	return (false);
}

bool	everyone_ate_enough(t_data *data)
{
	int	i;
	int	done;
	t_philo	*philos;

	i = 0;
	done = 0;
	philos = data->philos;
	if (data->params.max_meals <= 0)
		return (false);
	while (i < data->params.philos_count)
	{
		pthread_mutex_lock(&philos[i].meal_mutex);
		if (philos[i].meals >= data->params.max_meals)
			done++;
		pthread_mutex_unlock(&philos[i].meal_mutex);
		i++;
	}
	return (done == data->params.philos_count);
}

void	*monitoring(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;

	while (1)
	{
		if (philo_died(data) || everyone_ate_enough(data))
			break ;
	}
	pthread_mutex_lock(&data->state);
	data->stop = true;
	pthread_mutex_unlock(&data->state);
	return (NULL);
}