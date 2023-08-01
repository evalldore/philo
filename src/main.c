/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niceguy <niceguy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 02:17:36 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/01 09:05:08 by niceguy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

uint32_t	get_id()
{
	static uint32_t	index;
	return (++index);
}

/*bool	philo_can_eat(t_philo_state	*state, t_philo *philo)
{
	uint32_t	left_index;
	uint32_t	right_index;

	if (state->num_philos < 2)
		return (false);
	left_index = 0;
	while (left_index < state->num_philos)
	{
		if (state->forks[left_index])
		{
			right_index = left_index + 1;
			if (right_index == state->num_philos)
				right_index = 0;
			if (state->forks[right_index])
			{
				philo->fork_index = left_index;
				state->forks[left_index] = false;
				state->forks[right_index] = false;
				return (true);
			}
		}
		left_index++;
	}
	return (false);
}*/

uint64_t	get_time()
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (uint64_t)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void	*philo_func(void *ptr)
{
	t_philo_state	*state;
	t_philo			philo;

	(void)state;
	state = ptr;
	philo.id = get_id();
	philo.alive = true;
	philo.last_meal = get_time();
	while (philo.alive)
	{
		while (forks_get_available(state->forks, state->num_philos) < 0)
		{
			if ((get_time() - philo.last_meal) >= state->time_to_die)
			{
				printf(MSG_DIED, get_time(), philo.id);
				return (NULL);
			}
		}
		philo.fork_index = (uint32_t)forks_get_available(state->forks, state->num_philos);
		forks_set(&state->forks, false, philo.fork_index, state->num_philos);
		printf(MSG_EAT, get_time(), philo.id);
		usleep(state->time_to_eat * 1000);
		philo.last_meal = get_time();
		forks_set(&state->forks, true, philo.fork_index, state->num_philos);
		printf(MSG_SLEEP, get_time(), philo.id);
		usleep(state->time_to_sleep * 1000);
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
		pt->ret = pthread_create(&pt->pthread, NULL, philo_func, state);
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
		pthread_join(pt.pthread, NULL);
		num++;
	}
}

bool	philo_init(t_philo_state *state, int argc, char **argv)
{
	if (argc < 5)
		return false;
	state->start_time = get_time();
	state->num_eats = 0;
	state->num_philos = (uint32_t)ft_atoi(argv[1]);
	if (state->num_philos == 0)
		return (false);
	state->time_to_die = (uint32_t)ft_atoi(argv[2]);
	state->time_to_eat = (uint32_t)ft_atoi(argv[3]);
	state->time_to_sleep = (uint32_t)ft_atoi(argv[4]);
	if (argc > 5)
		state->num_eats = (uint32_t)ft_atoi(argv[5]);
	if (!forks_init(&(state->forks), state->num_philos))
		return (false);
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
