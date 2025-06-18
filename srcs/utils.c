/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 09:46:17 by arocca            #+#    #+#             */
/*   Updated: 2025/06/18 21:27:10 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <limits.h>
#include <stdlib.h>

bool	everyone_ate_enough(t_data *data)
{
	int	i;
	int	done;

	i = 0;
	done = 0;
	pthread_mutex_lock(&data->state);
	while (i < data->params.philos_count)
	{
		if (data->params.max_meals && data->philos[i].meals >= data->params.max_meals)
			done++;
		i++;
	}
	if (done == data->params.philos_count)
		data->stop = true;
	pthread_mutex_unlock(&data->state);
	return (done == data->params.philos_count);
}

bool	stopped(t_data *data)
{
	bool	stopped;

	pthread_mutex_lock(&data->state);
	stopped = data->stop;
	pthread_mutex_unlock(&data->state);
	return (stopped);
}

void	print(t_data *data, t_philo *philo, long ts, char *msg)
{
	if (stopped(data) || everyone_ate_enough(data))
		return ;
	pthread_mutex_lock(&data->printer);
	printf("%ld %d %s\n", ts, philo->id, msg);
	pthread_mutex_unlock(&data->printer);
}

static size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	exit_err(char *message)
{
	write(2, "\e[1m\e[31m", 9);
	write(2, message, ft_strlen(message));
	write(2, "\n\e[0m", 5);
	exit(1);
}

int	ft_atoi(const char *nptr)
{
	unsigned long long	res;
	int					sign;

	res = 0;
	sign = 1;
	if (!nptr || !*nptr)
		return (0);
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == ' ')
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			sign *= -1;
		nptr++;
	}
	while (*nptr >= '0' && *nptr <= '9')
	{
		res = res * 10 + (*nptr++ - '0');
		if (res > LLONG_MAX && sign < 0)
			return (0);
		if (res > LLONG_MAX)
			return (-1);
	}
	return (((int)res) * sign);
}
