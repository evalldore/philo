/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niceguy <niceguy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 06:27:15 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/02 08:11:06 by niceguy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	eat(t_philo_state *state, t_philo *philo)
{
	uint32_t		next_fork;
	
	next_fork = philo->id;
	if (next_fork >= state->num_philos)
		next_fork = 0;
	pthread_mutex_lock(&state->forks[philo->id - 1]);
	printf(MSG_FORK, get_time(state->start_time), philo->id);
	pthread_mutex_lock(&state->forks[next_fork]);
	printf(MSG_FORK, get_time(state->start_time), philo->id);
	printf(MSG_EAT, get_time(state->start_time), philo->id);
	usleep(state->time_to_eat * 1000);
	philo->last_meal = get_time(state->start_time);
	philo->num_meals++;
	pthread_mutex_unlock(&state->forks[philo->id - 1]);
	pthread_mutex_unlock(&state->forks[next_fork]);
}

void	*philo_routine(void *ptr)
{
	static uint32_t	index;
	t_philo_state	*state;
	t_philo			*philo;
	uint32_t		next_fork;

	state = ptr;
	philo = &state->philos[index];
	philo->id = ++index;
	philo->last_meal = get_time(state->start_time);
	philo->num_meals = 0;
	next_fork = philo->id;
	if (next_fork >= state->num_philos)
		next_fork = 0;
	while (state->simulating)
	{
		eat(state, philo);
		printf(MSG_SLEEP, get_time(state->start_time), philo->id);
		usleep(state->time_to_sleep * 1000);
		printf(MSG_THINK, get_time(state->start_time), philo->id);
	}
	return (NULL);
}
