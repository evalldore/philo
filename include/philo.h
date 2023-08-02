/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niceguy <niceguy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 02:18:35 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/01 19:12:25 by niceguy          ###   ########.fr       */
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
# include <sys/time.h>
# include "utils.h"
# include "forks.h"

# define MSG_FORK "%lu %u has taken a fork\n"
# define MSG_EAT "%lu %u is eating\n"
# define MSG_SLEEP "%lu %u is sleeping\n"
# define MSG_THINK "%lu %u is thinking\n"
# define MSG_DIED "%lu %u has died\n"
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

typedef struct s_philo_thread
{
	int			ret;
	pthread_t	pthread;
}	t_philo_thread;

typedef struct s_philo_state
{
	uint32_t		num_philos;
	uint32_t		num_eats;
	uint32_t		time_to_die;
	uint32_t		time_to_eat;
	uint32_t		time_to_sleep;
	uint64_t		start_time;
	pthread_mutex_t lock;
	t_philo_thread	*threads;
	bool			*forks;
}	t_philo_state;

typedef struct s_philo
{
	uint32_t	id;
	bool		alive;
	uint64_t	last_meal;
	uint32_t	fork_index;
}	t_philo;

#endif