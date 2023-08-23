/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evallee- <evallee-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 02:17:36 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/23 16:03:54 by evallee-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	ph_create_threads(t_state *s)
{
	uint32_t		i;

	i = 0;
	s->threads = malloc(sizeof(pthread_t) * s->num_philos);
	if (!s->threads)
		return (false);
	while (i < s->num_philos)
	{
		if (pthread_create(&s->threads[i], NULL, ph_routine, &s->philos[i]) < 0)
			return (false);
		i++;
	}
	return (true);
}

static bool	ph_create_philos(t_state *state)
{
	t_philo		*ph;
	uint32_t	i;

	state->philos = malloc(sizeof(t_philo) * state->num_philos);
	if (!state->philos)
		return (false);
	i = 0;
	while (i < state->num_philos)
	{
		ph = &(state->philos[i]);
		ph->simulating = true;
		ph->rules = state->rules;
		ph->id = i + 1;
		ph->last_meal = get_time(state->rules.start);
		ph->num_meals = 0;
		ph->death = &state->death;
		ph->print = &state->print;
		ph->num_philos = state->num_philos;
		ph->forks[0] = &state->forks[i];
		ph->forks[1] = &state->forks[(i + 1) % state->num_philos];
		i++;
	}
	return (true);
}

static bool	ph_init(t_state *state, int argc, char **argv)
{
	state->num_philos = (uint32_t)ft_atoi(argv[1]);
	state->rules.time_to_die = (uint32_t)ft_atoi(argv[2]);
	state->rules.time_to_eat = (uint32_t)ft_atoi(argv[3]);
	state->rules.time_to_sleep = (uint32_t)ft_atoi(argv[4]);
	if (state->num_philos <= 0 || state->rules.time_to_die <= 0 
		|| state->rules.time_to_eat <= 0 || state->rules.time_to_sleep <= 0)
		return (false);
	state->rules.num_eats = 0;
	if (argc == 6)
		state->rules.num_eats = (uint32_t)ft_atoi(argv[5]);
	state->rules.start = get_time(0);
	state->simulating = true;
	if (pthread_mutex_init(&state->death, NULL) != 0)
		return (false);
	if (pthread_mutex_init(&state->print, NULL) != 0)
		return (false);
	if (!forks_init(&state->forks, state->num_philos))
		return (false);
	if (!ph_create_philos(state))
		return (false);
	if (!ph_create_threads(state))
		return (false);
	return (true);
}

static void	ph_simulate(t_state *s)
{
	uint32_t		i;
	bool			ate_all;
	uint64_t		curr_time;

	while (s->simulating)
	{
		i = 0;
		ate_all = true;
		pthread_mutex_lock(&s->death);
		while (i < s->num_philos)
		{
			curr_time = get_time(s->rules.start);
			if ((curr_time - s->philos[i].last_meal) >= s->rules.time_to_die)
			{
				ph_terminate(s);
				printf(MSG_DIED, curr_time, s->philos[i].id);
				break ;
			}
			if (s->philos[i++].num_meals != s->rules.num_eats)
				ate_all = false;
		}
		if (s->rules.num_eats > 0 && ate_all)
			ph_terminate(s);
		pthread_mutex_unlock(&s->death);
	}
}

int	main(int argc, char **argv)
{
	t_state			state;
	uint32_t		i;

	if (argc < 5 || argc > 6)
		return (1);
	if (!ph_init(&state, argc, argv))
	{
		ph_clear(&state);
		return (1);
	}
	ph_simulate(&state);
	i = 0;
	while (i < state.num_philos)
		pthread_join(state.threads[i++], NULL);
	ph_clear(&state);
	return (0);
}
