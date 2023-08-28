/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evallee- <evallee-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 05:13:58 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/28 18:41:49 by evallee-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forks.h"
#include <stdio.h>

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

void	forks(pthread_mutex_t *f, pthread_mutex_t **p, uint32_t i, uint32_t n)
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
