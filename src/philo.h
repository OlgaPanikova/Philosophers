/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opanikov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 13:45:58 by lelichik          #+#    #+#             */
/*   Updated: 2024/08/03 20:47:22 by opanikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_philo
{
	int					num_philo;
	int					count_eat;
	long long			last_meal;
	pthread_t			th;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		last_meal_mutex;
	pthread_mutex_t		count_eat_mutex;
	struct s_general	*info;
}			t_philo;

typedef struct s_general
{
	int				count_philo;
	long long		time_die;
	long long		time_eat;
	long long		time_sleep;
	int				num_eat_before_exit;
	long long		start_time;
	int				died_philo;
	int				someone_died;
	int				max_eat;
	pthread_t		monitor_th;
	pthread_t		die_th;
	pthread_t		eat_th;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;
	pthread_mutex_t	died_mutex;
	t_philo			*philo;
}			t_general;

void		init_general(t_general *info, int ac, char **arg);


int			check_arguments(int ac, char **av);
int			check_num(char *str);
int			ft_atoi(const char *str);

void		clean_up(t_general *info);
void		error_exit(t_general *info, char *error_message);

long long	get_time(void);
long long	get_curr_time(t_general *info);
void		ft_usleep(int i);

int			born_philo(t_general *info);
int			check_died(t_philo *philo);

int			ph_thinking(t_philo *philo);
int			ph_sleeping(t_philo *philo);
int			ph_eating(t_philo *philo);

#endif