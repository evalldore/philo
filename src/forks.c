/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niceguy <niceguy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 05:13:58 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/26 21:53:46 by niceguy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forks.h"

bool	forks_init(pthread_mutex_t **forks, uint32_t num_forks)
{
	uint32_t	num;

	*forks = malloc(sizeof(pthread_mutex_t) * num_forks);
	if (!(*forks))
		return (false);
	num = 0;
	while (num < num_forks)
		pthread_mutex_init(&(*forks)[num++], NULL);
	return (true);
}

void	forks_clear(pthread_mutex_t **forks, uint32_t num_forks)
{
	uint32_t	i;

	if (!(*forks))
		return ;
	i = 0;
	while (i < num_forks)
		pthread_mutex_destroy(&(*forks)[i++]);
	free(*forks);
	*forks = NULL;
}

void	forks_assign(pthread_mutex_t *forks, pthread_mutex_t **philo, uint32_t i, uint32_t num)
{
	if ((i % 2) == 0)
	{
		philo[0] = &forks[i];
		philo[1] = &forks[(i + 1) % num];
		return ;
	}
	philo[0] = &forks[(i + 1) % num];
	philo[1] = &forks[i];
}
