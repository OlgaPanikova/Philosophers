/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelichik <lelichik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 23:24:43 by lelichik          #+#    #+#             */
/*   Updated: 2024/08/05 22:53:16 by lelichik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time(void)
{
	struct timeval	time;
	long long		res;

	gettimeofday(&time, NULL);
	res = (time.tv_sec * 1000 + time.tv_usec / 1000);
	return (res);
}

long long	get_curr_time(t_general *info)
{
	struct timeval	time;
	long long		res;

	gettimeofday(&time, NULL);
	res = (time.tv_sec * 1000 + time.tv_usec / 1000) - info->start_time;
	return (res);
}

void	ft_usleep(int i)
{
	long long	start;

	start = get_time();
	while (get_time() - start < i)
		usleep(i / 2);
}