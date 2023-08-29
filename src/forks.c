/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niceguy <niceguy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 05:13:58 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/29 00:11:44 by niceguy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forks.h"
#include <stdio.h>

bool	forks_init(t_fork **forks, uint32_t num_forks)
{
	uint32_t	i;

	*forks = malloc(sizeof(t_fork) * num_forks);
	if (!(*forks))
		return (false);
	i = 0;
	while (i < num_forks)
	{
		pthread_mutex_init(&(*forks)[i].mutex, NULL);
		(*forks)[i].check = true;
		i++;
	}
	return (true);
}

void	forks_clear(t_fork **forks, uint32_t num_forks)
{
	uint32_t	i;

	if (!(*forks))
		return ;
	i = 0;
	while (i < num_forks)
	{
		pthread_mutex_destroy(&(*forks)[i].mutex);
		i++;
	}
	free(*forks);
	*forks = NULL;
}

void	forks(t_fork *f, t_fork **p, uint32_t i, uint32_t n)
{
	uint32_t	next;

	next = (i + 1) % n;
	if ((i % 2) == 0)
	{
		p[0] = &f[i];
		p[1] = &f[next];
		return ;
	}
	p[0] = &f[next];
	p[1] = &f[i];
}
