/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evallee- <evallee-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 02:18:35 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/23 16:04:39 by evallee-         ###   ########.fr       */
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

# define MSG_FORK "%llu %u has taken a fork\n"
# define MSG_EAT "%llu %u is eating\n"
# define MSG_SLEEP "%llu %u is sleeping\n"
# define MSG_THINK "%llu %u is thinking\n"
# define MSG_DIED "%llu %u\033[0;31m has died\n\033[0;37m"
/*
◦ number_of_philosophers: The number of philosophers and also the number
of forks.
◦ time_to_die (in milliseconds): If a philosopher didn’t start eating time_to_die
milliseconds since the beginning of their last meal or the beginning of the sim-
ulation, they die.
◦ time_to_eat (in milliseconds): The time it takes for a philosopher to eat.
During that time, they will need to hold two forks.
◦ time_to_sleep (in milliseconds): The time a philosopher will spend sleeping.
◦ number_of_times_each_philosopher_must_eat (optional argument): If all
philosophers have eaten at least number_of_times_each_philosopher_must_eat
times, the simulation stops. If not specified, the simulation stops when a
philosopher dies.
*/

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
	pthread_mutex_t		*death;
	pthread_mutex_t		*print;
	pthread_mutex_t		*meals;
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
	pthread_mutex_t	death;
	pthread_mutex_t	print;
	pthread_mutex_t	meals;
}	t_state;

void	*ph_routine(void *ptr);
void	ph_print(t_philo *philo, char *msg, uint64_t time);
bool	ph_sleep(t_philo *philo, uint64_t delay);
bool	ph_is_alive(t_philo *philo);
void	ph_terminate(t_state *s);
void	ph_clear(t_state *state);

#endif