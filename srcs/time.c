/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 18:21:38 by arocca            #+#    #+#             */
/*   Updated: 2025/09/03 21:51:26 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>

unsigned long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000));
}

void	ms_wait(long ms)
{
	unsigned long	start;
	unsigned long	elapsed;
	long			to_wait;

	to_wait = ms;
	start = get_time();
	if (to_wait > 20)
		usleep((to_wait - 10) * 1000);
	while (1)
	{
		elapsed = get_time() - start;
		if (elapsed >= (unsigned long)ms)
			break ;
		if ((ms - elapsed) > 10)
			usleep(1000);
		else
			usleep(100);
	}
}

long	timestamp(unsigned long start)
{
	return (get_time() - start);
}
