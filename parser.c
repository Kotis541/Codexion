/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vokotera <vokotera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 09:47:21 by vokotera          #+#    #+#             */
/*   Updated: 2026/06/10 15:37:14 by vokotera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_arg_format(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (i < argc - 1)
	{
		j = 0;
		while (argv[i][j])
		{
			if (!(argv[i][j] >= '0' && argv[i][j] <= '9'))
			{
				printf("ERROR: You can add just numbers!\n");
				return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

int	check_arg(int argc, char **argv)
{
	if (argc > 9)
	{
		printf("ERROR: You added too many arguments\n");
		return (1);
	}
	if (argc < 9)
	{
		printf("ERROR: You need to add more arguments\n");
		return (1);
	}
	return (check_arg_format(argc, argv));
}

static int	init_dongles(t_data *data)
{
	int	i;

	data->dongles = malloc(sizeof(t_dongle) * data->number_of_coders);
	if (!data->dongles)
		return (1);
	i = -1;
	while (++i < data->number_of_coders)
		data->dongles[i].heap_data = NULL;
	i = 0;
	while (i < data->number_of_coders)
	{
		data->dongles[i].is_taken = 0;
		data->dongles[i].cooldown_end = 0;
		data->dongles[i].heap_size = 0;
		data->dongles[i].heap_data = malloc(sizeof(t_dongle_req)
				* data->number_of_coders);
		if (!data->dongles[i].heap_data)
			return (1);
		pthread_mutex_init(&data->dongles[i].lock, NULL);
		pthread_cond_init(&data->dongles[i].cond, NULL);
		i++;
	}
	return (0);
}

int	parse_arg(t_data *data, int argc, char **argv)
{
	if (check_arg(argc, argv))
		return (1);
	if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0)
	{
		printf("ERROR: You added wrong scheduler (fifo or edf)\n");
		return (1);
	}
	data->number_of_coders = atoi(argv[1]);
	data->time_to_burnout = atoi(argv[2]);
	data->time_to_compile = atoi(argv[3]);
	data->time_to_debug = atoi(argv[4]);
	data->time_to_refactor = atoi(argv[5]);
	data->number_of_compiles_required = atoi(argv[6]);
	data->dongle_cooldown = atoi(argv[7]);
	data->scheduler = (strcmp(argv[8], "edf") == 0);
	pthread_mutex_init(&data->dead_mutex, NULL);
	pthread_mutex_init(&data->log_mutex, NULL);
	return (init_dongles(data));
}
