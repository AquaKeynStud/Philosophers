/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 08:25:35 by arocca            #+#    #+#             */
/*   Updated: 2025/07/08 15:08:49 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
