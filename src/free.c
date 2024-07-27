/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opanikov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 19:42:10 by lelichik          #+#    #+#             */
/*   Updated: 2024/07/27 12:31:27 by opanikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_general_struct(t_general *info)
{
	int	i;
	
	i = 0;
	if (info == NULL)
		return ;
	if (info->forks)
	{
		while (i < info->count_philo)
		{
			pthread_mutex_destroy(&info->forks[i]);
			i++;
		}
		free(info->forks);
	}
	if (info->philo)
		free(info->philo); // навенрое надо добавить мьютекс дестрой для остального
	free(info);
}

void	error_exit(t_general *info, char *error_message)
{
	printf("%s\n", error_message);
	free_general_struct(info);
}
