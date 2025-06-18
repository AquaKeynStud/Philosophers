/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 18:21:38 by arocca            #+#    #+#             */
/*   Updated: 2025/06/18 19:10:14 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000));
}

void	ms_wait(long ms)
{
	long	now;
	long	start;

	start = get_time();
	if (ms > 10)
		usleep((ms - 10) * 1000);
	while (1)
	{
		now = get_time();
		if (now - start >= ms)
			break;
		usleep(100);
	}
}

long	timestamp(t_data *data)
{
	return (get_time() - data->start);
}
