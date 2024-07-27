/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lelichik <lelichik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 14:29:34 by lelichik          #+#    #+#             */
/*   Updated: 2024/07/22 14:52:06 by lelichik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int			i;
	long long	res;

	i = 0;
	res = 0;
	while (str[i] != '\0' && str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 +(str[i] - '0');
		i++;
	}
	if (res > 2147483647)
		return (-1);
	return ((int)res);
}

int	check_num(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		i++;
	}
	j = ft_atoi(str);
	if (j == -1 || j == 0)
		return (1);
	return (0);
}

int	check_arguments(int ac, char **av)
{
	int	i;
	
	i = 1;
	if (ac < 5 || ac > 6)
		return (1);
	while (i < ac)
	{
		if (check_num(av[i]))
			return (1);
		i++;
	}
	return (0);
}
