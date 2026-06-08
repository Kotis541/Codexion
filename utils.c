/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vokotera <vokotera@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 09:47:30 by vokotera          #+#    #+#             */
/*   Updated: 2026/06/08 09:47:30 by vokotera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	check_death(t_coder *coder)
{
	int	status;

	pthread_mutex_lock(&coder->data->dead_mutex);
	status = coder->data->is_dead;
	pthread_mutex_unlock(&coder->data->dead_mutex);
	return (status);
}

void	print_status(t_coder *coder, char *msg)
{
	long	time_diff;

	pthread_mutex_lock(&coder->data->log_mutex);
	time_diff = get_time() - coder->data->start_time;
	if (!check_death(coder))
		printf("%ld %d %s\n", time_diff, coder->id, msg);
	pthread_mutex_unlock(&coder->data->log_mutex);
}

void	free_dongles(t_data *data)
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
