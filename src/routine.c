/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niceguy <niceguy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 06:27:15 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/07 08:48:43 by niceguy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool check_simulate(t_philo_state *state)
{
	pthread_mutex_lock(&state->death);
	if (!state->simulating)
	{
		pthread_mutex_unlock(&state->death);
		return (false);
	}
	pthread_mutex_unlock(&state->death);
	return (true);
}

static bool pickup_fork(t_philo_state *state, uint32_t fork_index)
{
	pthread_mutex_lock(&state->forks[fork_index]);
	if (!state->simulating)
	{
		pthread_mutex_unlock(&state->forks[fork_index]);
		return (false);
	}
	return (true);
}

static bool	eat(t_philo_state *state, t_philo *philo)
{
	uint32_t		next_fork;
	
	next_fork = philo->id % state->num_philos;
	if (!pickup_fork(state, philo->id - 1))
		return (false);
	printf(MSG_FORK, get_time(state->start_time), philo->id);
	if (!pickup_fork(state, next_fork))
		return (false);
	printf(MSG_FORK, get_time(state->start_time), philo->id);
	if (!check_simulate(state))
		return (false);
	pthread_mutex_lock(&state->meals);
	if (!check_simulate(state))
		return (false);
	printf(MSG_EAT, get_time(state->start_time), philo->id);
	philo->last_meal = get_time(state->start_time);
	philo->num_meals++;
	pthread_mutex_unlock(&state->meals);
	usleep(state->time_to_eat * 1000);
	pthread_mutex_unlock(&state->forks[philo->id - 1]);
	pthread_mutex_unlock(&state->forks[next_fork]);
	return (check_simulate(state));
}

void	*ph_routine(void *ptr)
{
	static uint32_t	index;
	t_philo_state	*state;
	t_philo			*philo;

	state = ptr;
	philo = &state->philos[index++];
	while (1)
	{
		if (!check_simulate(state))
			return (NULL);
		if (!eat(state, philo))
			return (NULL);
		printf(MSG_SLEEP, get_time(state->start_time), philo->id);
		usleep(state->time_to_sleep * 1000);
		if (!check_simulate(state))
			return (NULL);
		printf(MSG_THINK, get_time(state->start_time), philo->id);
	}
	return (NULL);
}
