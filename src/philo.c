/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelichik <lelichik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 00:15:14 by lelichik          #+#    #+#             */
/*   Updated: 2024/08/03 14:46:11 by lelichik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// void *death_checker(void *arg)
// {
// 	t_general *info;
// 	int i;

// 	info = (t_general *)arg;
// 	while (1)
// 	{
// 		i = 0;
// 		while (i < info->count_philo)
// 		{
// 			if(check_died(&info->philo[i]))
// 				return (NULL);
// 			pthread_mutex_lock(&info->philo[i].last_meal_mutex);
// 			if ((get_curr_time(info) - info->philo[i].last_meal) > info->time_die)
// 			{
// 				pthread_mutex_lock(&info->died_mutex);
// 				info->someone_died = 1;
// 				pthread_mutex_unlock(&info->died_mutex);
// 				pthread_mutex_lock(&info->print);
// 				printf("Time: %lldms Philo: %d died\n", get_curr_time(info), info->philo[i].num_philo + 1);
// 				pthread_mutex_unlock(&info->print);
// 				pthread_mutex_unlock(&info->philo[i].last_meal_mutex);
// 				return (NULL);
// 			}
// 			pthread_mutex_unlock(&info->philo[i].last_meal_mutex);
// 			i++;
// 		}
// 	}
// 	return (NULL);
// }

// void *count_eat_checker(void *arg)
// {
// 	t_general *info;
// 	int i;
// 	int all_eaten;

// 	info = (t_general *)arg;
// 	while (1)
// 	{
// 		pthread_mutex_lock(&info->died_mutex);
// 		if (info->someone_died)
// 		{
// 			pthread_mutex_unlock(&info->died_mutex);
// 			return (NULL);
// 		}
// 		pthread_mutex_unlock(&info->died_mutex);
// 		all_eaten = 1;
// 		i = 0;
// 		while (i < info->count_philo)
// 		{
// 			pthread_mutex_lock(&info->philo[i].count_eat_mutex);
// 			if (info->philo[i].count_eat < info->num_eat_before_exit)
// 				all_eaten = 0;
// 			pthread_mutex_unlock(&info->philo[i].count_eat_mutex);
// 			i++;
// 		}
// 		if (all_eaten)
// 		{
// 			pthread_mutex_lock(&info->died_mutex);
// 			info->someone_died = 1;
// 			pthread_mutex_unlock(&info->died_mutex);
// 			return (NULL);
// 		}
// 	}
// 	return (NULL);
// }


void *monitor(void *arg)
{
    t_general *info;
    int i;
    int all_eaten;

    info = (t_general *)arg;
    while (1)
    {
        i = 0;
        all_eaten = 1;
        while (i < info->count_philo)
        {
            if (check_died(&info->philo[i]))
                return (NULL);

            // Проверка на время последнего приема пищи
            pthread_mutex_lock(&info->philo[i].last_meal_mutex);
            if ((get_curr_time(info) - info->philo[i].last_meal) > info->time_die)
            {
                pthread_mutex_lock(&info->died_mutex);
                info->someone_died = 1;
                pthread_mutex_unlock(&info->died_mutex);
                pthread_mutex_lock(&info->print);
                printf("Time: %lldms Philo: %d died\n", get_curr_time(info), info->philo[i].num_philo + 1);
                pthread_mutex_unlock(&info->print);
                pthread_mutex_unlock(&info->philo[i].last_meal_mutex);
                return (NULL);
            }
            pthread_mutex_unlock(&info->philo[i].last_meal_mutex);

            // Проверка на количество съеденной пищи
			if (info->num_eat_before_exit > 0)
			{
				pthread_mutex_lock(&info->philo[i].count_eat_mutex);
				if (info->philo[i].count_eat < info->num_eat_before_exit)
					all_eaten = 0;
				pthread_mutex_unlock(&info->philo[i].count_eat_mutex);
			}
			i++;
		}

        // Проверка, если все философы съели достаточное количество пищи
        if (info->num_eat_before_exit > 0 && all_eaten)
        {
            pthread_mutex_lock(&info->died_mutex);
            info->someone_died = 1;
            pthread_mutex_unlock(&info->died_mutex);
            return (NULL);
        }

        usleep(1000); // Sleep for a short period to reduce CPU usage
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
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->info->count_philo == 1)
	{
		while (1)
		{
			if(check_died(philo) != 0)
				return (NULL);
		}
	}
	if (philo->num_philo % 2 != 0)
		usleep(2500);
	while (1)
	{
		if (ph_eating(philo) != 0)
			return (NULL);
		if (ph_sleeping(philo) != 0)
			return (NULL);
		if (ph_thinking(philo) != 0)
			return (NULL);
	}
	return (NULL);
}

int born_philo(t_general *info)
{
	int	i;

	i = 0;
	while (i < info->count_philo)
	{
		if (pthread_create(&info->philo[i].th, NULL, &routine, (void*)&info->philo[i]) != 0)
		{
			error_exit(info, "Failed to create thread\n");
			return (1);
		}
		i++;
	}
	if (pthread_create(&info->monitor_th, NULL, &monitor, (void*)info) != 0)
	{
		error_exit(info, "Failed to create monitor thread\n");
		return (1);
	}
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
	if (pthread_join(info->monitor_th, NULL) != 0)
	{
		error_exit(info, "Monitor thread return with error\n");
		return (1);
	}
	return (0);
}


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
