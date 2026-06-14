/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vokotera <vokotera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 10:02:18 by vokotera          #+#    #+#             */
/*   Updated: 2026/06/10 15:37:00 by vokotera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_coders(t_data *data, t_coder **coders)
{
	int	i;
	int	next_dongle;

	*coders = malloc(sizeof(t_coder) * data->number_of_coders);
	if (!(*coders))
		return (1);
	i = 0;
	while (i < data->number_of_coders)
	{
		(*coders)[i].id = i + 1;
		(*coders)[i].data = data;
		(*coders)[i].compiles_done = 0;
		(*coders)[i].last_compile_time = data->start_time;
		pthread_mutex_init(&(*coders)[i].time_mutex, NULL);
		(*coders)[i].left_dongle = &data->dongles[i];
		next_dongle = (i + 1) % data->number_of_coders;
		(*coders)[i].right_dongle = &data->dongles[next_dongle];
		i++;
	}
	return (0);
}

static int	start_threads(t_data *data, t_coder *coders, pthread_t **threads,
			pthread_t *monitor)
{
	int	i;

	*threads = malloc(sizeof(pthread_t) * data->number_of_coders);
	if (!(*threads))
		return (1);
	data->start_time = get_time();
	i = 0;
	while (i < data->number_of_coders)
	{
		coders[i].last_compile_time = data->start_time;
		pthread_create(&(*threads)[i], NULL, &routine, &coders[i]);
		i++;
	}
	pthread_create(monitor, NULL, &monitor_routine, coders);
	return (0);
}

static void	join_and_free(t_data *data, t_coder *coders, pthread_t *threads,
			pthread_t monitor)
{
	int	i;

	pthread_join(monitor, NULL);
	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	free_system(data, coders, threads);
}

static t_data	*init_data(int argc, char **argv)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	if (parse_arg(data, argc, argv) != 0)
	{
		free(data);
		return (NULL);
	}
	data->is_dead = 0;
	data->start_time = get_time();
	return (data);
}

int	main(int argc, char **argv)
{
	t_data		*data;
	t_coder		*coders;
	pthread_t	*threads;
	pthread_t	monitor;

	coders = NULL;
	threads = NULL;
	data = init_data(argc, argv);
	if (!data)
		return (1);
	if (init_coders(data, &coders) != 0)
		return (free_system(data, coders, threads), 1);
	if (start_threads(data, coders, &threads, &monitor) != 0)
		return (free_system(data, coders, threads), 1);
	join_and_free(data, coders, threads, monitor);
	return (0);
}
