/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evallee- <evallee-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 05:17:05 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/28 17:32:28 by evallee-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FORKS_H
# define FORKS_H
# include <stdint.h>
# include <stdbool.h>
# include <stdlib.h>
# include <pthread.h>

bool	forks_init(pthread_mutex_t **forks, uint32_t num_forks);
void	forks_clear(pthread_mutex_t **forks, uint32_t num_forks);
void	forks(pthread_mutex_t *f, pthread_mutex_t **p, uint32_t i, uint32_t n);

#endif