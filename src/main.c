/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opanikov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 13:45:24 by lelichik          #+#    #+#             */
/*   Updated: 2024/08/03 20:46:56 by opanikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_forks(t_general *info)
{
	int	i;

	i = 0;
	info->forks = malloc(info->count_philo * sizeof(pthread_mutex_t));
	if(!info->forks)
	{
		error_exit(info, "Memory cannot be allocated");
		return (1);
	}
	while(i < info->count_philo)
	{
		if (pthread_mutex_init(&info->forks[i], NULL) != 0)
		{
			error_exit(info, "Failed to initialize mutex");
			return (1);
		}
		i++;
	}
	return (0);
}

int	init_philo(t_general *info)
{
	t_philo	*ph;
	int		i;

	i = 0;
	info->philo = malloc(info->count_philo * sizeof(t_philo));
	if(!info->philo)
	{
		error_exit(info, "Memory cannot be allocated");
		return (1);
	}
	while(i < info->count_philo)
	{
		ph = &info->philo[i];
		ph->num_philo = i;
		ph->right_fork = &info->forks[i];
		ph->left_fork = &info->forks[(i + 1) % info->count_philo];
		ph->info = info;
		ph->last_meal = 0;
		ph->count_eat = 0;
		pthread_mutex_init(&(info->philo[i].last_meal_mutex), NULL);
		pthread_mutex_init(&(info->philo[i].count_eat_mutex), NULL);
		i++;
	}
	return (0);
}

void	init_general(t_general *info, int ac, char **arg)
{
	info->count_philo = ft_atoi(arg[1]);
	info->time_die = ft_atoi(arg[2]);
	info->time_eat = ft_atoi(arg[3]);
	info->time_sleep = ft_atoi(arg[4]);
	if(ac == 6)
		info->num_eat_before_exit = ft_atoi(arg[5]);
	else
		info->num_eat_before_exit = 0;
	info->someone_died = 0;
	info->max_eat = 0;
	pthread_mutex_init(&info->print, NULL);
	pthread_mutex_init(&info->died_mutex, NULL);
}

int	main(int argc, char **argv)
{
	t_general *info;
	
	if(check_arguments(argc, argv) == 1)
	{
		printf("The arguments are specified incorrectly\n");
		return (1);
	}
	info = malloc(sizeof(t_general));
	if(!info)
		return (1);
	init_general(info, argc, argv);
	if((init_forks(info) != 0) || (init_philo(info) != 0))
		return (1);
	if(born_philo(info) != 0)
		return (1);
	clean_up(info);
	return (0);
}
