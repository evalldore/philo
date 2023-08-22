/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evallee- <evallee-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 02:17:36 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/22 16:51:57 by evallee-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	ph_create_threads(t_state *state)
{
	uint32_t		i;

	i = 0;
	state->threads = malloc(sizeof(pthread_t) * state->num_philos);
	if (!state->threads)
		return (false);
	while (i < state->num_philos)
	{
		if (pthread_create(&state->threads[i], NULL, ph_routine, &state->philos[i]) < 0)
			return (false);
		i++;
	}
	return (true);
}

static void	ph_assign_forks(t_state *state, uint32_t id)
{
	uint32_t	fid[2];

	fid[0] = id;
	fid[1] = (id + 1) % state->num_philos;
	if (id % 2)
	{
		fid[0] = fid[1];
		fid[1] = id;
	}
	state->philos[id].forks[0] = &state->forks[fid[0]];
	state->philos[id].forks[1] = &state->forks[fid[1]];
}

static bool	ph_create_philos(t_state *state)
{
	uint32_t	i;

	state->philos = malloc(sizeof(t_philo) * state->num_philos);
	if (!state->philos)
		return (false);
	i = 0;
	while (i < state->num_philos)
	{
		state->philos[i].simulating = true;
		state->philos[i].rules = state->rules;
		state->philos[i].id = i + 1;
		state->philos[i].last_meal = get_time(state->rules.start);
		state->philos[i].num_meals = 0;
		state->philos[i].death = &state->death;
		state->philos[i].print = &state->print;
		ph_assign_forks(state, i);
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

static void	ph_terminate(t_state *s)
{
	uint32_t	i;

	i = 0;
	s->simulating = false;
	while (i < s->num_philos)
		s->philos[i++].simulating = false;
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

static void	ph_clear(t_state *state)
{
	forks_clear(&state->forks, state->num_philos);
	if (state->philos)
		free(state->philos);
	if (state->threads)
		free(state->threads);
	pthread_mutex_destroy(&state->death);
	pthread_mutex_destroy(&state->print);
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
