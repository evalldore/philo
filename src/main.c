/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niceguy <niceguy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 02:17:36 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/02 04:45:33 by niceguy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_routine(void *ptr)
{
	static uint32_t	index;
	t_philo_state	*state;
	t_philo			philo;

	state = ptr;
	philo.id = ++index;
	philo.last_meal = get_time();
	while (state->simulating)
	{
		while (!forks_set(&state->forks, true, philo.id - 1, state->num_philos))
		{
			if ((get_time() - philo.last_meal) >= state->time_to_die)
			{
				printf(MSG_DIED, get_time(), philo.id);
				state->simulating = false;
				return (NULL);
			}
		}
		printf(MSG_FORK, get_time(), philo.id);
		printf(MSG_EAT, get_time(), philo.id);
		usleep(state->time_to_eat * 1000);
		philo.last_meal = get_time();
		forks_set(&state->forks, false, philo.id - 1, state->num_philos);
		printf(MSG_SLEEP, get_time(), philo.id);
		usleep(state->time_to_sleep * 1000);
		printf(MSG_THINK, get_time(), philo.id);
	}
	return (NULL);
}

bool	philo_create_threads(t_philo_state *state)
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

void	philo_join_threads(t_philo_state *state)
{
	t_philo_thread	pt;
	uint32_t		num;

	num = 0;
	while (num < state->num_philos)
	{
		pt = state->threads[num];
		pt.ret = pthread_join(pt.pthread, NULL);
		printf("%d: returns %d\n", num, pt.ret);
		num++;
	}
}

bool	philo_init(t_philo_state *state, int argc, char **argv)
{
	if (argc < 5)
		return false;
	state->start_time = get_time();
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
	state->simulating = true;
	if (!philo_create_threads(state))
		return (false);
	return (true);
}

int main(int argc, char **argv)
{
	t_philo_state			state;

	if (!philo_init(&state, argc, argv))
		return (1);
	philo_join_threads(&state);
	return (0);
}
