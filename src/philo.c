/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelichik <lelichik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 00:15:14 by lelichik          #+#    #+#             */
/*   Updated: 2024/07/26 16:43:33 by lelichik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int check_died(t_philo *philo)
{
	int	check_died;

	pthread_mutex_lock(&philo->info->died_mutex);
	if (get_curr_time(philo->info) - philo->last_meal > philo->info->time_die)
	{
		philo->info->someone_died = 1;
		pthread_mutex_lock(&philo->info->print);
		printf("Time: %lldms Philo: %d is died\n", get_curr_time(philo->info), philo->num_philo + 1);
		pthread_mutex_unlock(&philo->info->print);
		pthread_mutex_unlock(&philo->info->died_mutex);
		return (1);
	}
	check_died = philo->info->someone_died;
	pthread_mutex_unlock(&philo->info->died_mutex);
	return (check_died);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->num_philo % 2 != 0)
		usleep(2000);
	while(1)
	{
		//где-то нужно сделать проверку на ондго философа
		if(ph_eating(philo) != 0)
			return (NULL);
		if(ph_sleeping(philo) != 0)
			return (NULL);
		if(ph_thinking(philo) != 0)
			return (NULL);
		pthread_mutex_lock(&philo->count_eat_mutex);
		if (philo->count_eat >= philo->info->num_eat_before_exit && philo->info->num_eat_before_exit > 0)
		{
			pthread_mutex_unlock(&philo->count_eat_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->count_eat_mutex);
		pthread_mutex_lock(&philo->info->died_mutex);
		if (philo->info->someone_died != 0)
		{
			pthread_mutex_unlock(&philo->info->died_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->info->died_mutex);
	}
	return (NULL);
}


void	born_philo(t_general *info)
{
	int	i;

	i = 0;
	while(i < info->count_philo)
	{
		(pthread_create(&info->philo[i].th, NULL, &routine, (void*)&info->philo[i])); // подумать нужно ли добавить проверку на возврат не 0
		i++;
	}
	i = 0;
	while (i < info->count_philo)
	{
		pthread_join(info->philo[i].th, 0);
		i++;
	}
}