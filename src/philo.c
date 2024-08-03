/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opanikov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 00:15:14 by lelichik          #+#    #+#             */
/*   Updated: 2024/08/03 21:29:23 by opanikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	monitor(t_general *info)
{
	int	i;

	while (!info->max_eat)
	{
		i = 0;
		while (i < info->count_philo && !info->someone_died)
		{
			pthread_mutex_lock(&info->philo[i].last_meal_mutex);
			if ((get_curr_time(info) - info->philo[i].last_meal) > info->time_die)
			{
				pthread_mutex_unlock(&info->philo[i].last_meal_mutex);
				pthread_mutex_lock(&info->print);
				printf("Time: %lldms Philo: %d died\n", get_curr_time(info), info->philo[i].num_philo + 1);
				pthread_mutex_unlock(&info->print);
				pthread_mutex_lock(&info->died_mutex);
				info->someone_died = 1;
				pthread_mutex_unlock(&info->died_mutex);
			}
			pthread_mutex_unlock(&info->philo[i].last_meal_mutex);
			i++;
		}
		if (info->someone_died)
			break;
		i = 0;
		while (info->num_eat_before_exit && i < info->count_philo && info->philo[i].count_eat >= info->num_eat_before_exit)
			i++;
		if (i == info->count_philo)
			info->max_eat = 1;
	}
}

int check_died(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->died_mutex);
	if (philo->info->someone_died == 1)
	{
		pthread_mutex_unlock(&philo->info->died_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->info->died_mutex);
	return (0);
}

void *routine(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	if (philo->info->count_philo == 1)
	{
		while (1)
		{
			if (check_died(philo) != 0)
				return (NULL);
			ft_usleep(100);
		}
	}
	if (philo->num_philo % 2 != 0)
		usleep(2500);
	while (!philo->info->someone_died && !philo->info->max_eat)
	{
		if(ph_eating(philo))
			return (NULL);
		if(philo->info->someone_died == 1)
			return (NULL);
		if(ph_sleeping(philo))
			return (NULL);
		if(philo->info->someone_died == 1)
			return (NULL);
		if(ph_thinking(philo))
			return (NULL);
		if(philo->info->someone_died == 1)
			return (NULL);
	}
	return (NULL);
}

int born_philo(t_general *info)
{
	int	i;

	i = 0;
	info->start_time = get_time();
	while (i < info->count_philo)
	{
		if (pthread_create(&info->philo[i].th, NULL, &routine, (void *)&info->philo[i]) != 0)
		{
			error_exit(info, "Failed to create thread\n");
			return (1);
		}
		i++;
	}
	monitor(info);
	i = 0;
	while (i < info->count_philo)
	{
		if (pthread_join(info->philo[i].th, NULL) != 0)
		{
			error_exit(info, "Thread return with error\n");
			return (1);
		}
		i++;
	}
	return (0);
}


















// void	*monitor(void *arg)
// {
// 	t_general	*info;

// 	info = (t_general *)arg;
// 	int	i;

// 	while (!info->max_eat)
// 	{
// 		i = 0;
// 		while (i < info->count_philo && !info->someone_died)
// 		{
// 			pthread_mutex_lock(&info->philo[i].last_meal_mutex);
// 			if ((get_curr_time(info) - info->philo[i].last_meal) > info->time_die)
// 			{
// 				pthread_mutex_lock(&info->print);
// 				printf("Time: %lldms Philo: %d died\n", get_curr_time(info), info->philo[i].num_philo + 1);
// 				pthread_mutex_lock(&info->died_mutex);
// 				info->someone_died = 1;
// 				pthread_mutex_unlock(&info->died_mutex);
// 				return (NULL);
// 			}
// 			pthread_mutex_unlock(&info->philo[i].last_meal_mutex);
// 			i++;
// 		}
// 		if (info->someone_died)
// 			break;
// 		i = 0;
// 		while (info->num_eat_before_exit && i < info->count_philo && info->philo[i].count_eat >= info->num_eat_before_exit)
// 			i++;
// 		if (i == info->count_philo)
// 			info->max_eat = 1;
// 	}
// 	return (NULL);
// }

// int check_died(t_philo *philo)
// {
// 	pthread_mutex_lock(&philo->info->died_mutex);
// 	if (philo->info->someone_died == 1)
// 	{
// 		pthread_mutex_unlock(&philo->info->died_mutex);
// 		return (1);
// 	}
// 	pthread_mutex_unlock(&philo->info->died_mutex);
// 	return (0);
// }

// void	*routine(void *arg)
// {
// 	t_philo	*philo;

// 	philo = (t_philo *)arg;
// 	if (philo->info->count_philo == 1)
// 	{
// 		while (1)
// 		{
// 			if(check_died(philo) != 0)
// 				return (NULL);
// 		}
// 	}
// 	if (philo->num_philo % 2 != 0)
// 		ft_usleep(philo->info->time_sleep / 50);
// 	while (!philo->info->someone_died && !philo->info->max_eat)
// 	{
// 		ph_eating(philo);
// 		pthread_mutex_lock(&philo->info->print);
// 		printf("Time: %lldms Philo: %d is sleeping\n", get_curr_time(philo->info), philo->num_philo + 1);
// 		pthread_mutex_unlock(&philo->info->print);
// 		ft_usleep(philo->info->time_sleep);
// 		pthread_mutex_lock(&philo->info->print);
// 		printf("Time: %lldms Philo: %d is thinking\n", get_curr_time(philo->info), philo->num_philo + 1);
// 		pthread_mutex_unlock(&philo->info->print);
// 	}
// 	return (NULL);
// }

// int born_philo(t_general *info)
// {
// 	int	i;

// 	i = 0;
// 	while (i < info->count_philo)
// 	{
// 		if (pthread_create(&info->philo[i].th, NULL, &routine, (void*)&info->philo[i]) != 0)
// 		{
// 			error_exit(info, "Failed to create thread\n");
// 			return (1);
// 		}
// 		i++;
// 	}
// 	pthread_create(&info->monitor_th, NULL, &monitor, (void*)info);
// 	pthread_mutex_unlock(&info->print);
// 	i = 0;
// 	while (i < info->count_philo)
// 	{
// 		if (pthread_join(info->philo[i].th, NULL) != 0)
// 		{
// 			error_exit(info, "Thread return with error\n");
// 			return (1);
// 		}
// 		i++;
// 	}
// 	pthread_join(info->monitor_th, NULL);
// 	return (0);
// }


// int	born_philo(t_general *info)
// {
// 	int	i;

// 	i = 0;
// 	while(i < info->count_philo)
// 	{
// 		if(pthread_create(&info->philo[i].th, NULL, &routine, (void*)&info->philo[i]) != 0)
// 		{
// 			error_exit(info, "Failed to create thread\n");
// 			return (1);
// 		}
// 		i++;
// 	}
// 	pthread_create(&info->die_th, NULL, &death_checker, (void*)info);
// 	if (info->num_eat_before_exit > 0)
// 		pthread_create(&info->eat_th, NULL, &count_eat_checker, (void*)info);
// 	i = 0;
// 	while (i < info->count_philo)
// 	{
// 		if(pthread_join(info->philo[i].th, 0) != 0)
// 		{
// 			error_exit(info, "Thread return with error\n");
// 			return (1);
// 		}
// 		i++;
// 	}
// 	if (pthread_join(info->die_th, NULL) != 0)
// 	{
// 		error_exit(info, "Monitoring checker thread return with error\n");
// 		return (1);
// 	}
// 	if (info->num_eat_before_exit > 0)
// 		pthread_join(info->eat_th, NULL);
// 	return (0);
// }
// lok(is_dead)
// if(is_dead != 1)
// {
// 	unlok(is_dead)


// }
// unlok(is_dead)