/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niceguy <niceguy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 02:17:36 by niceguy           #+#    #+#             */
/*   Updated: 2023/07/31 08:35:34 by niceguy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*void *print_message(void *ptr)
{
	printf("%s\n", (char *)ptr);
	return (NULL);
}*/

/*int main(int argc, char **argv)
{
	pthread_t	threads[2];
	int			iret[2];
	char		*messages[2];

	(void)argc;
	(void)argv;
	messages[0] = "blast";
	messages[1] = "fast";
	iret[0] = pthread_create(&threads[0], NULL, print_message, messages[0]);
	iret[1] = pthread_create(&threads[1], NULL, print_message, messages[1]);
	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);
	printf("Thread 1 Returns: %d\n", iret[0]);
	printf("Thread 1 Returns: %d\n", iret[1]);
	return (0);
}*/

uint32_t	get_id()
{
	static uint32_t	index;
	return (index++);
}

/*bool	philo_sleep(uint32_t time, uint32_t start, uint32_t last_meal)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	while ((start + time) >= tv.tv_usec)
	{

	}
	return (true);
}*/

void	*philo_func(void *ptr)
{
	uint32_t		philo_id;
	t_philo			*state;
	bool			alive;
	struct timeval	tv;

	alive = true;
	state = ptr;
	philo_id = get_id();
	(void)state;
	while (alive)
	{
		gettimeofday(&tv, NULL);
		printf("Philo id: %u\n", philo_id);
		/*printf("Num philos:%d\n\t", state->num_philos);
		printf("Time to die:%d\n\t", state->time_to_die);
		printf("Time to eat:%d\n\t", state->time_to_eat);
		printf("Time to sleep:%d\n\t", state->time_to_sleep);
		printf("Num eat:%d\n", state->num_eats);*/
		alive = false;
	}
	return (NULL);
}

bool	philo_create_threads(t_philo *state)
{
	t_list			*node;
	t_philo_thread	*pt;
	uint32_t		num;

	num = state->num_philos;
	state->threads_list = NULL;
	while (num > 0)
	{
		pt = malloc(sizeof(t_philo_thread));
		if (!pt)
			return (false);
		pt->ret = pthread_create(&pt->pthread, NULL, philo_func, &state);
		node = malloc(sizeof(t_list));
		if (!node)
			return (false);
		node->content = pt;
		ft_lstadd_front(&state->threads_list, node);
		num--;
	}
	return (true);
}

void	philo_join_threads(t_list *list)
{
	t_philo_thread	*philo_thread;

	while (list)
	{
		philo_thread = list->content;
		pthread_join(philo_thread->pthread, NULL);
		list = list->next;
	}
}

bool	philo_init(t_philo *state, int argc, char **argv)
{
	if (argc < 5)
		return false;
	state->num_eats = 0;
	state->num_philos = (uint32_t)ft_atoi(argv[1]);
	if (state->num_philos == 0)
		return (false);
	state->time_to_die = (uint32_t)ft_atoi(argv[2]);
	state->time_to_eat = (uint32_t)ft_atoi(argv[3]);
	state->time_to_sleep = (uint32_t)ft_atoi(argv[4]);
	if (argc > 5)
		state->num_eats = (uint32_t)ft_atoi(argv[5]);
	if (!philo_create_threads(state))
	{
		ft_lstclear(&state->threads_list, free);
		return (false);
	}
	return (true);
}

int main(int argc, char **argv)
{
	t_philo			state;

	if (!philo_init(&state, argc, argv))
		return (1);
	philo_join_threads(state.threads_list);
	/*iret[0] = pthread_create(&threads[0], NULL, philo_func, &state);
	iret[1] = pthread_create(&threads[1], NULL, philo_func, &state);
	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);
	printf("thread %d returns: %d\n", 0, iret[0]);
	printf("thread %d returns: %d\n", 1, iret[1]);*/
	return (0);
}
