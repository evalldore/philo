/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niceguy <niceguy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 02:17:36 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/02 09:18:05 by niceguy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	ph_create_threads(t_philo_state *state)
{
	t_philo_thread	*pt;
	uint32_t		num;

	num = 0;
	state->threads = malloc(sizeof(t_philo_thread) * state->num_philos);
	if (!state->threads)
		return (false);
	while (num < state->num_philos)
	{
		pt = &state->threads[num];
		if (pthread_create(&pt->pthread, NULL, philo_routine, state) < 0)
			return (false);
		num++;
	}
	return (true);
}

static bool	ph_create_philos(t_philo_state *state)
{
	uint32_t	i;

	state->philos = malloc(sizeof(t_philo) * state->num_philos);
	if (!state->philos)
		return (false);
	i = 0;
	while (i < state->num_philos)
	{
		state->philos[i].id = i + 1;
		state->philos[i].last_meal = get_time(state->start_time);
		state->philos[i].num_meals = 0;
		i++;
	}
	return (true);
}

static bool	ph_init(t_philo_state *state, int argc, char **argv)
{
	if (argc < 5)
		return false;
	state->start_time = get_time(0);
	state->num_philos = (uint32_t)ft_atoi(argv[1]);
	if (state->num_philos == 0)
		return (false);
	state->time_to_die = (uint32_t)ft_atoi(argv[2]);
	state->time_to_eat = (uint32_t)ft_atoi(argv[3]);
	state->time_to_sleep = (uint32_t)ft_atoi(argv[4]);
	state->num_eats = 0;
	if (argc > 5)
		state->num_eats = (uint32_t)ft_atoi(argv[5]);
	if (!forks_init(&(state->forks), state->num_philos))
		return (false);
	if (!ph_create_philos(state))
		return (false);
	state->simulating = true;
	pthread_mutex_init(&state->death, NULL);
	if (!ph_create_threads(state))
		return (false);
	return (true);
}

static void	ph_simulate(t_philo_state *state)
{
	uint32_t		i;
	t_philo			*philo;

	while (state->simulating)
	{
		i = 0;
		while (i < state->num_philos)
		{
			philo = &state->philos[i];
			pthread_mutex_lock(&state->death);
			if ((get_time(state->start_time) - philo->last_meal) >= state->time_to_die)
			{
				printf(MSG_DIED, get_time(state->start_time), philo->id);
				state->simulating = false;
			}
			pthread_mutex_unlock(&state->death);
			if (!state->simulating)
				break;
			i++;
		}
	}
}

int main(int argc, char **argv)
{
	t_philo_state	state;
	uint32_t		i;

	if (!ph_init(&state, argc, argv))
		return (1);
	ph_simulate(&state);
	i = 0;
	while (i < state.num_philos)
	{
		//state.threads[i].ret = pthread_join(state.threads[i].pthread, NULL);
		pthread_detach(state.threads[i].pthread);
		pthread_mutex_destroy(&state.forks[i]);
		i++;
	}
	free(state.forks);
	free(state.philos);
	free(state.threads);
	pthread_mutex_destroy(&state.death);
	return (0);
}
