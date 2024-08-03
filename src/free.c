/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opanikov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 19:42:10 by lelichik          #+#    #+#             */
/*   Updated: 2024/08/03 20:46:41 by opanikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean_up(t_general *info)
{
	int	i;

	if (info->forks != NULL)
	{
		i = 0;
		while (i < info->count_philo)
		{
			pthread_mutex_destroy(&info->forks[i]);
			i++;
		}
		free(info->forks);
	}
	if (info->philo != NULL)
	{
		i = 0;
		while (i < info->count_philo)
		{
			pthread_mutex_destroy(&info->philo[i].last_meal_mutex);
			pthread_mutex_destroy(&info->philo[i].count_eat_mutex);
			i++;
		}
		free(info->philo);
	}
	pthread_mutex_destroy(&info->print);
	pthread_mutex_destroy(&info->died_mutex);
}

void	error_exit(t_general *info, char *error_message)
{
	printf("%s\n", error_message);
	clean_up(info);
}
