/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niceguy <niceguy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 02:18:35 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/26 23:33:00 by niceguy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdint.h>
# include <stdbool.h>
# include <unistd.h>
# include <stdatomic.h>
# include <sys/time.h>
# include "utils.h"
# include "forks.h"

# define MSG_FORK "%lu %u has taken a fork\n"
# define MSG_EAT "%lu %u is eating\n"
# define MSG_SLEEP "%lu %u is sleeping\n"
# define MSG_THINK "%lu %u is thinking\n"
# define MSG_DIED "%lu %u\033[0;31m has died\n\033[0;37m"

typedef struct s_rules
{
	uint32_t		num_eats;
	uint32_t		time_to_die;
	uint32_t		time_to_eat;
	uint32_t		time_to_sleep;
	uint64_t		start;
}	t_rules;

typedef struct s_philo
{
	uint32_t			id;
	uint64_t			last_meal;
	uint64_t			num_meals;
	uint64_t			num_philos;
	pthread_mutex_t		*forks[2];
	pthread_mutex_t		*print;
	pthread_mutex_t		lock;
	bool				simulating;
	t_rules				rules;
}	t_philo;

typedef struct s_state
{
	bool			simulating;
	uint32_t		num_philos;
	t_rules			rules;
	pthread_t		*threads;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;
}	t_state;

void	*ph_routine(void *ptr);
void	ph_print(t_philo *philo, char *msg, uint64_t time);
bool	ph_sleep(t_philo *philo, uint64_t delay);
bool	ph_is_alive(t_philo *philo);
void	ph_terminate(t_state *s);
void	ph_clear(t_state *state);

#endif