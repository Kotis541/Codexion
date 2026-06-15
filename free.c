/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vokotera <vokotera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 11:09:00 by vokotera          #+#    #+#             */
/*   Updated: 2026/06/15 13:12:48 by vokotera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	free_dongles(t_data *data)
{
	int	i;

	if (!data->dongles)
		return ;
	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_destroy(&data->dongles[i].lock);
		pthread_cond_destroy(&data->dongles[i].cond);
		if (data->dongles[i].heap_data)
			free(data->dongles[i].heap_data);
		i++;
	}
	free(data->dongles);
}

void	free_system(t_data *data, t_coder *coders, pthread_t *threads)
{
	int	i;

	if (threads)
		free(threads);
	if (coders)
	{
		i = -1;
		while (++i < data->number_of_coders)
			pthread_mutex_destroy(&coders[i].time_mutex);
		free(coders);
	}
	free_dongles(data);
	pthread_mutex_destroy(&data->dead_mutex);
	pthread_mutex_destroy(&data->log_mutex);
	free(data);
}
