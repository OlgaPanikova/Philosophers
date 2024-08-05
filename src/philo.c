/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelichik <lelichik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 00:15:14 by lelichik          #+#    #+#             */
/*   Updated: 2024/08/05 22:33:21 by lelichik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *monitor_thread(void *arg)
{
    t_general *info = (t_general *)arg;
    int i;

    while (1)
	{
        i = 0;
        while (i < info->count_philo && !info->someone_died)
		{
            pthread_mutex_lock(&info->philo[i].last_meal_mutex);
            if ((get_curr_time(info) - info->philo[i].last_meal) > info->time_die)
			{
                pthread_mutex_unlock(&info->philo[i].last_meal_mutex);
                pthread_mutex_lock(&info->died_mutex);
                info->someone_died = 1;
                pthread_mutex_unlock(&info->died_mutex);
                pthread_mutex_lock(&info->print);
                printf("Time: %lldms Philo: %d died\n", get_curr_time(info), info->philo[i].num_philo + 1);
                pthread_mutex_unlock(&info->print);
                return (NULL);
            }
            pthread_mutex_unlock(&info->philo[i].last_meal_mutex);
            i++;
        }
        pthread_mutex_lock(&info->died_mutex);
        if (info->someone_died == 1)
		{
            pthread_mutex_unlock(&info->died_mutex);
            return (NULL);
        }
        pthread_mutex_unlock(&info->died_mutex);
        i = 0;
        while (info->num_eat_before_exit && i < info->count_philo)
		{
            pthread_mutex_lock(&info->philo[i].count_eat_mutex);
            if (info->philo[i].count_eat < info->num_eat_before_exit)
			{
                pthread_mutex_unlock(&info->philo[i].count_eat_mutex);
                break ;
            }
            pthread_mutex_unlock(&info->philo[i].count_eat_mutex);
            i++;
        }
        if (i == info->count_philo)
		{
            pthread_mutex_lock(&info->max_eat_mutex);
            info->max_eat = 1;
            pthread_mutex_unlock(&info->max_eat_mutex);
            return (NULL);
        }
    }
	return (NULL);
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
	pthread_mutex_lock(&philo->info->max_eat_mutex);
		if (philo->info->max_eat)
		{
			pthread_mutex_unlock(&philo->info->max_eat_mutex);
			return (1);
		}
	pthread_mutex_unlock(&philo->info->max_eat_mutex);
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
	if (philo->num_philo % 2)
		usleep(2500);
	while (1)
	{
		if (ph_eating(philo))
			return (NULL);
		if (check_died(philo) != 0)
			return (NULL);
		if (ph_sleeping(philo))
			return (NULL);
		if (check_died(philo) != 0)
			return (NULL);
		if (ph_thinking(philo))
			return (NULL);
		if (check_died(philo) != 0)
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
		pthread_mutex_lock(&info->philo[i].last_meal_mutex);
        info->philo[i].last_meal = get_curr_time(info);
        pthread_mutex_unlock(&info->philo[i].last_meal_mutex);
		if (pthread_create(&info->philo[i].th, NULL, &routine, (void *)&info->philo[i]) != 0)
		{
			error_exit(info, "Failed to create thread\n");
			return (1);
		}
		i++;
	}
	pthread_create(&info->monitor_th, NULL, &monitor_thread, (void *)info);
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
	if(pthread_join(info->monitor_th, NULL) != 0)
	{
		error_exit(info, "Monitor return with error\n");
			return (1);
	}
	return (0);
}
