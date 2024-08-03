/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   live.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelichik <lelichik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 18:15:52 by lelichik          #+#    #+#             */
/*   Updated: 2024/08/03 01:36:42 by lelichik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ph_thinking(t_philo *philo)
{
	if(check_died(philo))
		return (1);
	pthread_mutex_lock(&philo->info->print);
	printf("Time: %lldms Philo: %d is thinking\n", get_curr_time(philo->info), philo->num_philo + 1);
	pthread_mutex_unlock(&philo->info->print);
	if(check_died(philo))
		return (1);
	return (0);
}

int	ph_sleeping(t_philo *philo)
{
	if(check_died(philo))
		return (1);
	pthread_mutex_lock(&philo->info->print);
	printf("Time: %lldms Philo: %d is sleeping\n", get_curr_time(philo->info), philo->num_philo + 1);
	pthread_mutex_unlock(&philo->info->print);
	ft_usleep(philo->info->time_sleep);
	if(check_died(philo))
		return (1);
	return (0);
}

int	ph_eating(t_philo *philo)
{
	if(check_died(philo))
		return (1);
	pthread_mutex_lock(philo->right_fork);
	if(check_died(philo))
	{
		pthread_mutex_unlock(philo->right_fork);
		return (1);
	}
	pthread_mutex_lock(philo->left_fork);
	if(check_died(philo))
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		return (1);
	}
	pthread_mutex_lock(&philo->info->print);
	printf("Time: %lldms Philo: %d has taken a forks\n", get_curr_time(philo->info), philo->num_philo + 1);
	printf("Time: %lldms Philo: %d is eating\n", get_curr_time(philo->info), philo->num_philo + 1);
	pthread_mutex_unlock(&philo->info->print);
	ft_usleep(philo->info->time_eat);
	pthread_mutex_lock(&philo->last_meal_mutex);
	philo->last_meal = get_curr_time(philo->info);
	pthread_mutex_unlock(&philo->last_meal_mutex);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_lock(&philo->count_eat_mutex);
	philo->count_eat++;
	pthread_mutex_unlock(&philo->count_eat_mutex);
	return (0);
}
