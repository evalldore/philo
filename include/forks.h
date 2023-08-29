/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niceguy <niceguy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 05:17:05 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/29 00:11:00 by niceguy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FORKS_H
# define FORKS_H
# include <stdint.h>
# include <stdbool.h>
# include <stdlib.h>
# include <pthread.h>

typedef struct s_fork
{
	pthread_mutex_t	mutex;
	bool			check;
}	t_fork;

bool	forks_init(t_fork **forks, uint32_t num_forks);
void	forks_clear(t_fork **forks, uint32_t num_forks);
void	forks(t_fork *f, t_fork **p, uint32_t i, uint32_t n);

#endif