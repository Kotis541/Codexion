/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vokotera <vokotera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 09:47:25 by vokotera          #+#    #+#             */
/*   Updated: 2026/06/10 15:16:35 by vokotera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	take_dongles(t_coder *coder, long priority)
{
	if (coder->id % 2 == 0)
	{
		if (!request_dongle(coder, coder->left_dongle, priority))
			return (0);
		print_status(coder, "has taken a dongle");
		if (!request_dongle(coder, coder->right_dongle, priority))
			return (release_dongle(coder->left_dongle, 0), 0);
		print_status(coder, "has taken a dongle");
	}
	else
	{
		if (!request_dongle(coder, coder->right_dongle, priority))
			return (0);
		print_status(coder, "has taken a dongle");
		if (!request_dongle(coder, coder->left_dongle, priority))
			return (release_dongle(coder->right_dongle, 0), 0);
		print_status(coder, "has taken a dongle");
	}
	return (1);
}

static int	coder_action(t_coder *coder)
{
	pthread_mutex_lock(&coder->time_mutex);
	coder->last_compile_time = get_time();
	pthread_mutex_unlock(&coder->time_mutex);
	print_status(coder, "is compiling");
	usleep(coder->data->time_to_compile * 1000);
	release_dongle(coder->left_dongle, coder->data->dongle_cooldown);
	release_dongle(coder->right_dongle, coder->data->dongle_cooldown);
	if (check_death(coder))
		return (1);
	print_status(coder, "is debugging");
	usleep(coder->data->time_to_debug * 1000);
	if (check_death(coder))
		return (1);
	print_status(coder, "is refactoring");
	usleep(coder->data->time_to_refactor * 1000);
	return (0);
}

static void	*single_coder(t_coder *coder)
{
	pthread_mutex_lock(&coder->left_dongle->lock);
	print_status(coder, "has taken a dongle");
	while (!check_death(coder))
		usleep(1000);
	pthread_mutex_unlock(&coder->left_dongle->lock);
	return (NULL);
}

static int	routine_loop(t_coder *coder)
{
	long	priority;

	if (coder->data->number_of_compiles_required > 0
		&& coder->compiles_done >= coder->data->number_of_compiles_required)
		return (1);
	priority = get_time();
	if (coder->data->scheduler == 1)
		priority = coder->last_compile_time + coder->data->time_to_burnout;
	if (!take_dongles(coder, priority))
		return (1);
	if (coder_action(coder))
		return (1);
	pthread_mutex_lock(&coder->time_mutex);
	coder->compiles_done++;
	pthread_mutex_unlock(&coder->time_mutex);
	usleep(1000);
	return (0);
}

void	*routine(void *args)
{
	t_coder	*coder;

	coder = (t_coder *)args;
	if (coder->data->number_of_coders == 1)
		return (single_coder(coder));
	if (coder->id % 2 != 0)
		usleep(10000);
	else
		usleep(coder->id * 1000);
	while (!check_death(coder))
	{
		if (routine_loop(coder))
			break ;
	}
	return (NULL);
}
