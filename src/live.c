/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   live.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelichik <lelichik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 18:15:52 by lelichik          #+#    #+#             */
/*   Updated: 2024/08/05 22:37:31 by lelichik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ph_thinking(t_philo *philo)
{
    pthread_mutex_lock(&philo->info->print);
    if (check_died(philo) != 0)
    {
        pthread_mutex_unlock(&philo->info->print);
		return (1);
    }
	printf("Time: %lldms Philo: %d is thinking\n", get_curr_time(philo->info), philo->num_philo + 1);
	pthread_mutex_unlock(&philo->info->print);
	return (0);
}

int	ph_sleeping(t_philo *philo)
{
	if (check_died(philo) != 0)
		return (1);
    pthread_mutex_lock(&philo->info->print);
	printf("Time: %lldms Philo: %d is sleeping\n", get_curr_time(philo->info), philo->num_philo + 1);
	pthread_mutex_unlock(&philo->info->print);
    if (check_died(philo) != 0)
		return (1);
	ft_usleep(philo->info->time_sleep);
	return (0);
}

int check_and_lock_forks(t_philo *philo)
{
    pthread_mutex_lock(philo->left_fork);
    pthread_mutex_lock(&philo->info->print);
    if (check_died(philo) != 0)
    {
        pthread_mutex_unlock(philo->left_fork);
        pthread_mutex_unlock(&philo->info->print);
        return (1);
    }
    printf("Time: %lldms Philo: %d has taken fork\n", get_curr_time(philo->info), philo->num_philo + 1);
    pthread_mutex_unlock(&philo->info->print);
    pthread_mutex_lock(philo->right_fork);
    pthread_mutex_lock(&philo->info->print);
    if (check_died(philo) != 0)
    {
        pthread_mutex_unlock(philo->left_fork);
        pthread_mutex_unlock(philo->right_fork);
        pthread_mutex_unlock(&philo->info->print);
        return (1);
    }
    printf("Time: %lldms Philo: %d has taken fork\n", get_curr_time(philo->info), philo->num_philo + 1);
    pthread_mutex_unlock(&philo->info->print);
    return (0);
}


int ph_eating(t_philo *philo)
{
    if (check_died(philo) != 0)
        return (1);
    if (check_and_lock_forks(philo) != 0)
        return (1);
    pthread_mutex_lock(&philo->info->print);
    printf("Time: %lldms Philo: %d is eating\n", get_curr_time(philo->info), philo->num_philo + 1);
    pthread_mutex_unlock(&philo->info->print);
    pthread_mutex_lock(&philo->last_meal_mutex);
    philo->last_meal = get_curr_time(philo->info);
    pthread_mutex_unlock(&philo->last_meal_mutex);
    ft_usleep(philo->info->time_eat);
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
    pthread_mutex_lock(&philo->count_eat_mutex);
    philo->count_eat++;
    pthread_mutex_unlock(&philo->count_eat_mutex);
    return (0);
}

