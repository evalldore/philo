/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evallee- <evallee-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 06:31:33 by niceguy           #+#    #+#             */
/*   Updated: 2023/08/25 18:54:00 by evallee-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H
# include <sys/time.h>
# include <stdint.h>
# include <stdlib.h>
# include <stdbool.h>

long		ft_atoi(const char *str);
uint64_t	get_time(uint64_t start);
bool		ft_check_args(int argc, char **argv);

#endif