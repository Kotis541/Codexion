/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vokotera <vokotera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 09:47:18 by vokotera          #+#    #+#             */
/*   Updated: 2026/06/10 10:05:05 by vokotera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	wake_all_dongles(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->number_of_coders)
	{
		pthread_mutex_lock(&data->dongles[i].lock);
		pthread_cond_broadcast(&data->dongles[i].cond);
		pthread_mutex_unlock(&data->dongles[i].lock);
	}
}

static int	trigger_death(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->log_mutex);
	if (!check_death(coder))
	{
		printf("%ld %d burned out\n", get_time() - coder->data->start_time,
			coder->id);
		pthread_mutex_lock(&coder->data->dead_mutex);
		coder->data->is_dead = 1;
		pthread_mutex_unlock(&coder->data->dead_mutex);
		wake_all_dongles(coder->data);
	}
	pthread_mutex_unlock(&coder->data->log_mutex);
	return (1);
}

static int	check_burnout(t_coder *coder, int *all_compiled)
{
	long	last;
	int		is_done;
	int		required;

	pthread_mutex_lock(&coder->time_mutex);
	last = coder->last_compile_time;
	required = coder->data->number_of_compiles_required;
	is_done = (required > 0 && coder->compiles_done >= required);
	if (!is_done)
		*all_compiled = 0;
	pthread_mutex_unlock(&coder->time_mutex);
	if (!is_done && (get_time() - last >= coder->data->time_to_burnout))
		return (trigger_death(coder));
	return (0);
}

void	*monitor_routine(void *args)
{
	t_coder	*coders;
	int		i;
	int		all_compiled;

	coders = (t_coder *)args;
	while (1)
	{
		i = -1;
		all_compiled = 1;
		while (++i < coders->data->number_of_coders)
		{
			if (check_burnout(&coders[i], &all_compiled))
				return (NULL);
		}
		if (coders->data->number_of_compiles_required > 0 && all_compiled)
		{
			pthread_mutex_lock(&coders->data->dead_mutex);
			coders->data->is_dead = 1;
			pthread_mutex_unlock(&coders->data->dead_mutex);
			wake_all_dongles(coders->data);
			return (NULL);
		}
		usleep(1000);
	}
}
