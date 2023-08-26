/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niceguy <niceguy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 05:17:05 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/26 14:09:33 by niceguy          ###   ########.fr       */
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
void	forks_assign(pthread_mutex_t **forks, pthread_mutex_t **philo, uint32_t i, uint32_t num);

#endif